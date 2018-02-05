
# The MIT license, the text of which is below, applies to the entire project in general.
# Project includes some third party libraries or modules that are licensed
# differently; the corresponding subfolder contains the license that applies in
# that case.


# Copyright (c) 2017 Bitcoin Trading Bot Project by Hongji Yang

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys
import os
import json
import ast
import math
import time
sys.path.append('/usr/local/lib/python2.7/site-packages')
from coinbase.wallet.client import Client
from pygame import mixer
from slackclient import SlackClient


api_key = ''
api_secret = ''
account_id = ''
slack_token = ""


tx_fee = 0.01

NEW_FILE_INTERVAL = 
MAX_ONE_TIME_USD_SELL = 
MIN_ONE_TIME_USD_SELL =

MAX_ONE_TIME_BUY_LIMIT =

MAX_TOTAL_BUY_LIMIT = 
MAX_BUY_PRICE = 

SELL_ALL_LOW_THREAD_USD = 
ONE_TIME_BUYING_AMOUNT = 


SELL_ALL_THREAD =
CB_VERSION = '20-12'

start_time = str(int(time.time()))

class Cb_tools:

	def __init__(self, obj):

		self.client = obj
		self.sc = SlackClient(slack_token)
		self.current_bought_usd = 0

		#f_name = 'Logs/price history ' + start_time + '.txt'
		#self.f = open(f_name,"a+")

	def obj_to_dic(self, obj):

		string = json.dumps(obj)
		dic = ast.literal_eval(string)

		return dic

	def get_blance(self):

		account = self.client.get_account(account_id)

		#account = self.obj_to_dic(account)
		string = json.dumps(account)
		dic = json.loads(string)

		return dic['balance']['amount']

	def get_sell_price(self):

		sell_price = self.obj_to_dic(self.client.get_sell_price(currency_pair = 'BTC-USD'))
		#self.f.write("%s\r\n" % sell_price['amount'])
		return sell_price['amount']

	def get_buy_price(self):

		buy_price = self.obj_to_dic(self.client.get_buy_price(currency_pair = 'BTC-USD'))
		#self.f.write("%s\r\n" % sell_price['amount'])
		return buy_price['amount']


	def get_spot_price(self):

		spot_price = self.obj_to_dic(self.client.get_spot_price(currency_pair = 'BTC-USD'))
		#self.f.write("%s\r\n" % sell_price['amount'])
		return spot_price['amount']


	# def get_sell_rate():

	# 	price = self.client.get_sell_price(currency_pair = 'BTC-USD')


	def sell_total(self):

		subtotal = float(self.get_sell_price()) * float(self.get_blance())
		total = subtotal * (1 - tx_fee)

		return int(math.ceil(total))


	def sell(self, amount):


		if amount < MAX_ONE_TIME_USD_SELL and amount > MIN_ONE_TIME_USD_SELL or amount == MIN_ONE_TIME_USD_SELL:

			sell = self.client.sell('',
                   amount = str(amount),
                   currency="USD",
                   payment_method="")

			print sell


	def sell_all(self):

		sell_amount = self.sell_total() - 2
		#sell_amount = 2


		sell = self.client.sell('',
               amount = str(sell_amount),
               currency="USD",
               payment_method="")

		print sell
		# print sell_amount
		# print type(sell_amount)

	def sold_out_nose(self):

		while True:

			mixer.init()

			sound = mixer.Sound('beep.wav')
			time.sleep(1)
			sound.play()
			time.sleep(1)
			sound.play()
			time.sleep(1)
			sound.play()
			time.sleep(1)
			os.system('say "All the Bitcoins has been sold out "')
			time.sleep(1)

	def slack_msg(self, msg):

		self.sc.api_call(
			"chat.postMessage",
			channel="#general",
			text= msg
		)


	def buy_with_usd(self, usd_amount):


		if usd_amount > MAX_ONE_TIME_BUY_LIMIT or not self.current_bought_usd < MAX_TOTAL_BUY_LIMIT:

			return

		else:

			buy = self.client.buy('',
	               total = str(usd_amount),
	               currency="USD",
	               payment_method="")

			self.current_bought_usd += usd_amount

			msg = 'Bought BTC with USD: ', str(usd_amount), 'total USB spent:',self.current_bought_usd
			self.slack_msg(msg)

			return buy


	def get_quote_buy(self, usd_amount):

		try:

			buy = self.client.buy('',
		               total = str(usd_amount),
		               currency="USD",
		               payment_method="", quote = 'true')


			quote = json.loads(json.dumps(buy))

			quote = str( float(quote['subtotal']['amount'])/float(quote['amount']['amount']))

			#quote = self.obj_to_dic(buy)

			return int(math.floor(float(quote)))

		except:

			return 0

	def get_quote_sell(self, usd_amount):

		try:

			buy = self.client.sell('',
		               total = str(usd_amount),
		               currency="USD",
		               payment_method="", quote = 'true')


			quote = json.loads(json.dumps(buy))

			quote = str( float(quote['subtotal']['amount'])/float(quote['amount']['amount']))

			#quote = self.obj_to_dic(buy)

			return int(math.floor(float(quote)))


		except:

			return 0


class sell_bitcoin:

	def __init__(self):

		self.SOLD_OUT = False

	def self_bitcoin_at_price_USD(self, sell_price_usd):


		client = Client(api_key, api_secret, api_version = CB_VERSION)
		cb_tools = Cb_tools(client)

		msg = "Selling all bitcoins at price:$" + str(sell_price_usd) + "\nRetype to confirmed_price:\n"
		confirmed_price = raw_input(msg)

		if int(confirmed_price) == sell_price_usd:

			pass

		else:

			return
		
		#f_name = 'Logs/Log file ' + start_time + '.txt'
		#f= open(f_name,"a+")

		#f_sell_price = 'Logs/Prices/sell price recorder history ' + start_time + '.txt'
		#f_sell = open(f_sell_price,"a+")

		#f_buy_price = 'Logs/Prices/buy price recorder history ' + start_time + '.txt'
		#f_buy = open(f_buy_price,"a+")

		#print cb_tools.get_blance()
		#print cb_tools.get_sell_price()

		#print cb_tools.sell_total()


		#usr = client.get_current_user()
		#print usr

		#account = client.get_account('')
		#account = client.get_accounts()
		#print account

		#rates = client.get_exchange_rates(currency = 'BTC')
		# buy_price = json.dumps(client.get_buy_price(currency_pair = 'BTC-USD'))
		# sell_price = json.dumps(client.get_sell_price(currency_pair = 'BTC-USD'))

		# buy_price = ast.literal_eval(buy_price)
		# sell_price = ast.literal_eval(sell_price)



		#print buy_price
		#print sell_price

		#sell_price = cb_tools.obj_to_dic(client.get_buy_price(currency_pair = 'BTC-USD'))
		#print sell_price['amount']


		#txs = client.get_sells('')
		#txs = client.get_buys('')

		#print txs
		

		#pms = client.get_payment_methods()
		#print pms

		# sell = client.sell('',
	 #                   amount="2",
	 #                   currency="USD",
	 #                   payment_method="")

		# print sell

		current_balance = cb_tools.sell_total()

		#cb_tools.slack_msg('#############################\n\tTrading bot has started\n#############################')


		while  current_balance > 10 and not self.SOLD_OUT:

			msg =  time.ctime(time.time()) + ': Current total BTC balance:' +  str(current_balance)
			print '#######################'
			print msg
			#f.write("%s\r\n" % msg)

			#sell_price = cb_tools.obj_to_dic(client.get_sell_price(currency_pair = 'BTC-USD'))
			#f_sell.write("%s\r\n" % sell_price['amount'])
			#buy_price = cb_tools.obj_to_dic(client.get_buy_price(currency_pair = 'BTC-USD'))
			#f_buy.write("%s\r\n" % sell_price['amount'])
			
			#spot_price = cb_tools.obj_to_dic(client.get_spot_price(currency_pair = 'BTC-USD'))


			#print 'sell: ', sell_price['amount'], 'buy: ', buy_price['amount'], 'spot: ', spot_price['amount']

			# if int(math.ceil(float(buy_price['amount']))) < MAX_BUY_PRICE:

			# 	buy = cb_tools.buy_with_usd(ONE_TIME_BUYING_AMOUNT)
			# 	print buy

			quote_price_sell = cb_tools.get_quote_sell(current_balance)

			print 'Quote sell price: $', quote_price_sell
			print 'SELL_ALL_LOW_THREAD_USD: $', SELL_ALL_LOW_THREAD_USD

			if SELL_ALL_LOW_THREAD_USD < quote_price_sell:

				try:

					msg =  'tying to sale all'
					print msg
					#f.write("%s\r\n" % msg)

					sell = client.sell(account_id,
				               amount=str(current_balance),
				               currency="USD",
				               payment_method="d")
					time.sleep(2)
					self.SOLD_OUT = True
					msg = '\n########################\nALL BTC SOUL OUT\n###########################\n'
					cb_tools.slack_msg(msg)
					print msg
					#f.write("%s\r\n" % msg)

					#cb_tools.sold_out_nose()

					#while True:


						#time.sleep(5)


				except:

					msg = 'sale failed'
					print msg
					#f.write("%s\r\n" % msg)

					time.sleep(1)

			else:

					pass
					# msg = '#########\nprice in safe region\n########\n' + str(current_balance - SELL_ALL_THREAD) + ' USD above sell price'
					# print msg
					# f.write("%s\r\n" % msg)

					# current_balance = cb_tools.sell_total()

					# if time.time() - int(start_time) > NEW_FILE_INTERVAL:

					# 	f_sell.close()
					# 	start_time =(str(int(time.time())))
					# 	f_sell_price = 'Logs/sell price recorder history ' + start_time + '.txt'
					# 	f_sell = open(f_sell_price,"a+")

					# 	f_buy.close()
					# 	start_time =(str(int(time.time())))
					# 	f_buy_price = 'Logs/buy price recorder history ' + start_time + '.txt'
					# 	f_buy = open(f_buy_price,"a+")

					time.sleep(2)





if __name__ == "__main__":



	sell_bot = sell_bitcoin()

	sell_bot.self_bitcoin_at_price_USD(SELL_ALL_LOW_THREAD_USD)












