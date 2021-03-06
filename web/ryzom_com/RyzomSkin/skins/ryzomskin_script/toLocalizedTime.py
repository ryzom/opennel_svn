## Script (Python) "toLocalizedTime"
##bind container=container
##bind context=context
##bind namespace=
##bind script=script
##bind subpath=traverse_subpath
##parameters=time=None, long_format=None, hour=None
##title=
##
# The time parameter must be either a string that is suitable for
# initializing a DateTime or a DateTime object.
# Returns a localized string.

tool = context.translation_service
time = tool.ulocalized_time(time, long_format, context, domain='RyzomSkin')

if long_format and time and hour:
   time += ' UTC'

if long_format and time and not hour:
   time = ' '.join(time.split()[0:4])

return time
