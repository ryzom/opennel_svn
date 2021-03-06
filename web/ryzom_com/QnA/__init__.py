from Products.Archetypes.public import process_types, listTypes
from Products.CMFCore.CMFCorePermissions import setDefaultRoles,ManagePortal
from Products.CMFCore import utils
from Products.CMFCore.DirectoryView import registerDirectory
from config import *

AddQnA = 'Add QnA object'
setDefaultRoles( AddQnA, ( 'Manager', 'Reviewer' ) )

registerDirectory(SKINS_DIR, GLOBALS)

def initialize(context):
	import QnA

	content_types, constructors, ftis = process_types(
		listTypes(PROJECTNAME),
		PROJECTNAME)

	utils.ContentInit(
		PROJECTNAME + ' Content',
		content_types = content_types,
		permission = AddQnA,
		extra_constructors = constructors,
		fti = ftis,
	).initialize(context)
