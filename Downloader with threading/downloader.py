# Authur: Hongji Yang. Mar 2018
import sys, shutil
import httplib
sys.path.append('/usr/local/lib/python2.7/site-packages')
from urlparse import urlparse 
from threading import Thread
import hashlib
import os
import time

#Setup static variables
url = 'https://storage.googleapis.com/vimeo-test/work-at-vimeo-2.mp4'
MAX_THREADS_COUNT = 45
MAX_ATTEMPT = 5
MAX_MEM = 1024*1024*5

class Downloader:
	def __init__(self, chunk_size_in_MB, bench = False):
		# URL to target file
		self.url = url
		# Size of chunk to grab each time
		self.chunk = 1024*1024*chunk_size_in_MB
		# Output file name
		self.file_name = url.split('/')[-1]
		# Parse the url to usabable format
		o = urlparse(self.url)
		# Establish connection to file sever
		conn = httplib.HTTPConnection(o.netloc)
		# Get header for information on the target file
		conn.request("GET", o.path)
		resp = conn.getresponse()
		header = dict(resp.getheaders())
		# Now we have the information, gracefully close the connection
		conn.close()
		# Pass information from header to internal variables
		self.file_len = int(header['x-goog-stored-content-length'])
		self.threads_count = self.file_len//self.chunk + 1
		self.etag = header['etag'].strip('"')
		self.is_bytes = header['accept-ranges'] == 'bytes'
		# Make sure threads count doesn't exceed limit
		# Connction starts to break around 50 threads for this particular sever
		if self.threads_count > MAX_THREADS_COUNT and not bench:
			raise ValueError('Threads count exceeds limit, use a larger chunk size and retry')
			sys.exit(1)
		# Create a place to temperorily store chunk files, if it does not already exist
		if not os.path.exists('Sample_files'):
   			os.makedirs('Sample_files')

   	# Get data from the offset byte to the offset+chunk byte
	def get_byte_data(self, url, offset, chunk):
		#parse url and establish connection
		o = urlparse(url)
		conn = httplib.HTTPConnection(o.netloc)
		headers = {}
		headers['Range'] = 'bytes=' + str(offset) + '-' + str(offset + chunk)
		try:
			# Get data
			conn.request("GET", o.path, headers = headers)
			resp = conn.getresponse()
			data = resp.read()
		except:
			print 'Sever unavailable, please try again later'
			sys.exit(1)
		# Create temperory chunk file to store data on the disk
		temp_file_name = 'Sample_files/' + 'temp_file_' + str(offset) + '_' + str(offset + chunk)
		with open(temp_file_name, 'w') as f:
			f.write(data)
		# Close up objects so they gets cleaned up
		f.close()
		conn.close()
		return resp.status

	def download_with_threads(self):
		try:
			offset = 0
			threads = []

			print 'Download in progress...'
			# Creating as many threads as necessary
			while offset < self.file_len:
				t = Thread(target = self.get_byte_data, args = (url, offset, self.chunk))
				threads.append(t)
				offset += self.chunk + 1

			# Start threads
			for t in threads:
				t.start()
			# Wait for threads to finish
			for t in threads:
				t.join()
		except:
			# One or more threads failed to complete download, start over
			print 'Download with threads failed, restart in 5s'
			time.sleep(5)
			#Threads tend to timeout with higher number of threads, decreasing threads
			self.chunk = self.chunk//2
			self.download_with_threads()

	# Without threading
	def download_at_once(self):

		offset = 0
		try:
			# Only latest chunk of data is stored in the mempry
			while True:
				status = self.get_byte_data(self.url, offset, self.chunk)
				#416 Requested Range Not Satisfiable, download complete
				if status == 416:
					print "Download successful"
					break
				offset += chunk + 1
		except:
			print 'Download failed, resume download in 5s'
			time.sleep(5)
			#Last successful chunk end point. New chunk starting point
			self.resume_on_error(offset)

	def resume_on_error(self, offset):
		try:
			# Only latest chunk of data is stored in the mempry
			while True:
				status = self.get_byte_data(self.url, offset, self.chunk)
				#416 Requested Range Not Satisfiable, download complete
				if status == 416:
					print "Download successful"
					break
				offset += chunk + 1
			return
		except:
			print 'Download failed, resume download in 5s'
			time.sleep(5)
			# Recursive till all chunks downloaded, resume from the last sucessful chunk fetch
			self.resume_on_error(offset)

	# Called by download_with_threads() to merge temperory chunk files into one output file
	def merge_files(self):
		file_list = []
		threads = []
		offset = 0
		try:
			# Figure out how many files need to be merged
			while offset < self.file_len:
				file = 'temp_file_' + str(offset) + '_' + str(offset + self.chunk)
				file_list.append(file)
				offset += self.chunk + 1
			# Create output file with correct name
			with open(self.file_name, 'w') as f:
			# Merge all temperory chunk files in order, one by one
			    for file in file_list:
			    	# Skip special entries
			    	if not file[0:4] == 'temp':
			    		continue
			    	path = 'Sample_files/' + file
			        with open(path) as f_temp:
				        data = f_temp.read()
				        f.write(data)
			# Close up file pointers so they don't stack up in the memory
			f.close()
			f_temp.close()
		except:
			print 'Internal error, please contact Hongji at homjiyang@gmail.com'
			sys.exit(1)

    # Compute output file MD5 hash, compare the hash with etag included in the header
	def validate_MD5_hash(self):
		try:
			# Create hash object
			m = hashlib.md5()
			# Start digesting data
			with open(self.file_name, 'rb') as f:
			    while True:
			        block = f.read(self.chunk)
			        if not block:
			        	# Stop when reach the end
			            break
			        # Compute hash
			        m.update(block)
			# Validate hash
			return m.hexdigest() == self.etag
		except:
			print 'Internal error, please contact Hongji at homjiyang@gmail.com'
			sys.exit(1)

	def start(self):
		# If sever support byte range, download with threading
		if self.is_bytes:
			# Try five times beofre exit with error
			for attempt in range(MAX_ATTEMPT):
				print 'Starting download'
				self.download_with_threads()
				self.merge_files()
				if 	self.validate_MD5_hash():
					print "Download completed!"
					self.clean_up()
					exit(0)
			raise ValueError('Download with threads failed')
			sys.exit(1)

		# If sever does not support byte range, download without threading
		else:
			# Try five times before exiting with error
			for attempt in range(MAX_ATTEMPT):
				self.download_at_once()
				self.merge_files()
				if 	self.validate_MD5_hash():
					print "Download completed!"
					self.clean_up()
					exit(0)
			raise ValueError('Download without threads failed')
			sys.exit(1)

	# Clean up temperory files and folder created for download with threading
	def clean_up(self):
		if os.path.isdir('Sample_files'):
			shutil.rmtree('Sample_files')
		else:
			print 'Directory is clean, exiting'
			sys.exit(0)

class Agent():
	def __init__(self):
		# Nothing to initilize here
		pass
	# User interface
	def run(self):
		print '########################################################################'
		print '                Hello V! Welcome to url video downloader'
		print '########################################################################'
		raw_input('Press enter to start download \n-->')
		# Default 25MB chunk size, configurable thrrough unit test class
		downloader = Downloader(25)
		downloader.start()

if __name__ == '__main__':
	agent = Agent()
	agent.run()




