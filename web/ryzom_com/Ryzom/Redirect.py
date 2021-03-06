try:
    from Products.LinguaPlone.public import *
except ImportError: 
    from Products.Archetypes.public import *
from Products.CMFCore import CMFCorePermissions
from config import PROJECTNAME
import re

RedirectSchema = BaseSchema.copy() + Schema((
	StringField('remote_url',
	        required=True,
        	searchable=True,
	        default = "http://",
        	validators = ('isURL',),
        	widget = StringWidget(label = "URL",)
	),
    ),)


class Redirect(BaseContent):
	"""Direct Redirection"""

	schema = RedirectSchema
	meta_type = portal_type = 'Redirect'
	_at_rename_after_creation = True

	actions = (
		{ 'id': 'view',
		'name': 'View',
		'action': 'string:${object_url}/redirect_view',
		'permissions': (CMFCorePermissions.View,)
		},
	)

#	def setTitle(self, value, **kwargs):
#		if not value and self.id:
#			value = self.id
#		else:			
#			try:
#				self.setId(re.sub('[^A-Za-z0-9_-]', '', re.sub(' ', '-', value)).lower())
#			except:
#				pass
#		self.getField('title').set(self, value, **kwargs)			


registerType(Redirect,PROJECTNAME)
