import xml.etree.ElementTree as ET
import sys

filename = sys.argv[1]

tree = ET.parse(filename)    
root = tree.getroot()

temp = []
count = 0

dict_ = {}
for packet in root:
    for proto in packet:
        if proto.get('name') == "smtp":
            for field in proto:
                if field.get('name') == "smtp.command_line":
                    string = field.get('showname')
                    if string.startswith('Command Line: mail FROM'):
                        count = count + 1
                        length = len(string)
                        length = length - 13
                        dict_["Sender Address"] = string.split('<')[1].split('>')[0]


                    if string.startswith('Command Line: rcpt TO'):
                        length = len(string)
                        length = length - 5
                        dict_["Receiver Address"] = string[23:length]

        if proto.get('name') == "imf":
            for field in proto:
                if field.get('name') == "imf.subject":
                    dict_["subject"] = field.get('showname')
                if field.get('name') == "imf.message_text":
                    for in_field in field:
                        dict_["mail body"] = in_field.get('show')
                        temp.append(dict_)
                        dict_={}
print("Total number of mails sent are:-",count)
print(temp)
