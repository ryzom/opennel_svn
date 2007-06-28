#!/usr/bin/python
from string import join
from string import split
import urllib2
def get_servers_status():
	try:
		server_data=urllib2.urlopen('http://atys.ryzom.com/serverstatus/status.php').read()
	except:
		return [('ATS','CLOSED'),]

	x=0
	res=[]
	for i in server_data.split('\n'):
		try:
			data = i.split('|')
			server = data[0].split(' ')[0]
			if server=='Advanced':
				server='ATS'
			if data[1]=='1':
				status = 'OPEN'
			elif data[1]=='2':
				status = 'LOCKED'
			else:
				status = 'CLOSED'
			res+=((server,status),)
		except:
			pass 
	return res
