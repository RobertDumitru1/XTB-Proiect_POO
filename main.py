import time

import yfinance
import json
import random
import time

companies = {}
reader = open("companies.txt", "r")
line = reader.readline()
symbol = line.strip()[1:]
company = yfinance.Ticker(symbol)
name, price, dividents = company.info.get("shortName"), company.info.get("regularMarketPrice"), company.info.get("trailingAnnualDividendYield")
if line[0] == 'S':
    companies[symbol] = {"type":"stock", "name": name, "price": price + random.randint(1, 9), "dividents": dividents}
else:
    companies[symbol] = {"type":"derivate", "name": name, "price": price + random.randint(1, 10), "leverage": random.random() * 50, "swap_fee": random.random() * 10}
reader.close()
with open("cmake-build-debug/companies_data_copy.json", "w") as writer:
    json.dump(companies, writer, indent=4)
print("done")


