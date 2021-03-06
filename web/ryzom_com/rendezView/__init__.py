from Products.Archetypes.public import process_types, listTypes

# CMF imports
from Products.CMFCore import utils
from Products.CMFCore.CMFCorePermissions import setDefaultRoles,AddPortalContent
from Products.CMFCore.DirectoryView import registerDirectory
from config import *

AddRendezView = 'rendezView : Add new Event - rendezView'
setDefaultRoles( AddRendezView, ( 'Manager', 'Reviewer','Owner', ) )
registerRendezView = 'rendezView : register to a event - rendezView'
setDefaultRoles( registerRendezView, ( 'Member','Owner', ) )

registerDirectory(SKINS_DIR, GLOBALS)

def initialize(context):
	import rendezView
	import participant
	import participantworkflow

	content_types, constructors, ftis = process_types(
		listTypes(PROJECTNAME),
		PROJECTNAME)
	
	allTypes = zip(content_types, constructors)
	for atype, constructor in allTypes:
		if atype.meta_type in ('rendezView'):
			kind = "%s: %s" % (config.PROJECTNAME, atype.meta_type)
			utils.ContentInit(
			kind,
			content_types      = (atype,),
 			permission         = AddRendezView,
 			extra_constructors = (constructor,),
 			fti                = ftis,
			).initialize(context)
		else:
			kind = "%s: %s" % (config.PROJECTNAME, atype.meta_type)
			utils.ContentInit(
			kind,
			content_types      = (atype,),
 			permission         = registerRendezView,
 			extra_constructors = (constructor,),
 			fti                = ftis,
			).initialize(context)
