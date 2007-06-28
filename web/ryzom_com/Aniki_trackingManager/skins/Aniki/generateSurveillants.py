## Script (Python) "newPatchNote"
##bind container=container
##bind context=context
##bind namespace=
##bind script=script
##bind subpath=traverse_subpath
##parameters=nb, lang=None,type=None,format=None,url, desc=None
##title=
##

format = (format,)
type = (type,)
lang = (lang,)
path = '/'.join(context.getPhysicalPath())
result = context.portal_catalog(
			meta_type=['Surveillant',],
			path={'query':path, 'level': 0},
			sort_on='Date',
                        sort_order='reverse',
			)
lastId = result[0].getId

#creation en boucle a partir de lastId+1 jusqu'a lastId+nb

r = range(int(lastId)+1,int(lastId)+int(nb)+1)
for i in r:
	newid = str(i)
	if len(newid) <3:
		newid = '00'+newid
	elif len(newid) <2:
		newid = '0'+newid
	print newid
	context.invokeFactory(id=newid, type_name='Surveillant', title=newid, langs = lang, types = type, formats = format,description=desc, urlRedirection=url)
	new_obj = getattr(context, newid)
	new_obj.setTitle(newid)
	new_obj.setLangs(lang)
	new_obj.setFormats(format)
	new_obj.setTypes(type)
	new_obj.setUrlRedirection(url)

#publication
#plustard

#container.REQUEST.RESPONSE.redirect(context.absolute_url())

return printed
