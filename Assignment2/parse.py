# packet->proto->frame

#First run this commmands in Terminal:
#1. pip3 install python-geoip-python3
#2. pip3 install python-geoip-geolite2
#3. pip3 install pycountry

from geoip import geolite2
import sys
import pycountry
import xml.etree.ElementTree as ET    	   #ElementTree
tree = ET.parse(sys.argv[1])
root = tree.getroot()

ip = set()                                 #set to store ip's

for packet in root:

	for proto in packet:
		flag=0;

		for field in proto:
			if(field.get('showname') != None and field.get('showname').find('Internet.org')!=-1):      #via: Internet.org proxy
				flag=1;
				print(field.get('showname'))

		if(flag and proto.get("name")=="http"):				#filtering out required ip's
			for field in proto:
				if(field.get("name") == "http.x_forwarded_for"):
					ip.add(field.get("show"))              #adding ip to the set 

					
user_count={}							#dictionary to store user count for countries
for x in ip:
	geo_loc = geolite2.lookup(x)                           #getting geolocation from ip (geo_loc.country is country code)
	
	if geo_loc is not None and geo_loc.country is not None:
		country = pycountry.countries.get(alpha_2=geo_loc.country)    #finding coutry_name from country_code
		user_count[country.name]=user_count.get(country.name,0)+1;	  #increasing user count for 
	

count=0

text = ''														#saving county_name and no.of people to text(comma separated)
for country in user_count.keys():
	print("{: >37}".format(country),"          ",user_count[country])
	text += ' '.join(country.split(',')) + ',' + str(user_count[country]) + '\n'
	count+=1

print("Total Number of country: ", count)

with open('data.csv','w') as f:									#writing text to csv file
    f.write(text)

