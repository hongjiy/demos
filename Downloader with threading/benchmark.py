from downloader_copy import Downloader
import time

class Unit_test():

	def __init__(self, chunk_min, chunk_max):
		self.chunk_min = chunk_min
		self.chunk_max = chunk_max


	def benchmark(self):

		benchmark = []

		for size in range(self.chunk_min,self.chunk_max):

			tic = time.time()
			#downloader = Downloader(size, True)
			downloader = Downloader(0.1, True)
			#downloader.file_len = 1024*1024*11
			downloader.download_with_threads()
			toc = time.time()

			dic = {}
			dic['size'] = size
			dic['time'] = toc - tic

			benchmark.append(dic)


		print benchmark

if __name__ == '__main__':


	unit_test = Unit_test(1,2)
	unit_test.benchmark()





