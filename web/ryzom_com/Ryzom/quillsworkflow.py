# 
#
# Generated by dumpDCWorkflow.py written by Sebastien Bigaret
# Original workflow id/title: quills_workflow/Quills Workflow
# Date: 2006/05/05 11:15:19.227 GMT+2
#
# WARNING: this dumps does NOT contain any scripts you might have added to
# the workflow, IT IS YOUR RESPONSABILITY TO MAKE BACKUPS FOR THESE SCRIPTS.
#
# The following scripts have been detected and should be backed up:
# - moveToWeblogRoot (External Method)
# - moveToArchive (External Method)
# 
"""
Programmatically creates a workflow type
"""
__version__ = "$Revision: 1.3 $"[11:-2]

from Products.CMFCore.WorkflowTool import addWorkflowFactory

from Products.DCWorkflow.DCWorkflow import DCWorkflowDefinition

def setupQuills_workflow(wf):
    "..."
    wf.setProperties(title='Quills Workflow (ryzom)')

    for s in ['private', 'visible', 'draft', 'pending', 'published', 'public']:
        wf.states.addState(s)
    for t in ['make_public', 'hide', 'publish', 'draft', 'submit', 'reject', 'retract', 'make_draft', 'publish_in_place']:
        wf.transitions.addTransition(t)
    for v in ['action', 'review_history', 'actor', 'comments', 'time']:
        wf.variables.addVariable(v)
    for l in ['reviewer_queue']:
        wf.worklists.addWorklist(l)
    for p in ('Modify portal content', 'View', 'Access contents information', 'Change portal events'):
        wf.addManagedPermission(p)
        

    ## Initial State
    wf.states.setInitialState('draft')

    ## States initialization
    sdef = wf.states['private']
    sdef.setProperties(title="""""",
                       transitions=('draft',))
    sdef.setPermission('Modify portal content', 0, ['Manager', 'Owner'])
    sdef.setPermission('View', 0, ['Manager', 'Owner'])
    sdef.setPermission('Access contents information', 0, ['Manager', 'Owner'])
    sdef.setPermission('Change portal events', 0, ['Manager', 'Owner'])

    sdef = wf.states['visible']
    sdef.setProperties(title="""""",
                       transitions=('draft',))
    sdef.setPermission('Modify portal content', 0, ['Manager', 'Owner'])
    sdef.setPermission('View', 0, ['Manager', 'Owner'])
    sdef.setPermission('Access contents information', 0, ['Manager', 'Owner'])
    sdef.setPermission('Change portal events', 0, ['Manager', 'Owner'])

    sdef = wf.states['draft']
    sdef.setProperties(title="""""",
                       transitions=('hide', 'publish', 'submit'))
    sdef.setPermission('Modify portal content', 0, ['Manager', 'Owner'])
    sdef.setPermission('View', 0, ['Manager', 'Owner', 'Reviewer'])
    sdef.setPermission('Access contents information', 0, ['Manager', 'Owner', 'Reviewer'])
    sdef.setPermission('Change portal events', 0, ['Manager', 'Owner'])

    sdef = wf.states['pending']
    sdef.setProperties(title="""""",
                       transitions=('hide', 'publish', 'reject', 'retract'))
    sdef.setPermission('Modify portal content', 0, ['Manager', 'Reviewer'])
    sdef.setPermission('View', 0, ['Manager', 'Owner', 'Reviewer'])
    sdef.setPermission('Access contents information', 0, ['Manager', 'Owner', 'Reviewer'])
    sdef.setPermission('Change portal events', 0, ['Manager', 'Reviewer'])

    sdef = wf.states['published']
    sdef.setProperties(title="""""",
                       transitions=('hide', 'make_draft', 'reject', 'retract'))
    sdef.setPermission('Modify portal content', 0, ['Manager', 'Owner'])
    sdef.setPermission('View', 0, ['Anonymous', 'Authenticated', 'Manager', 'Member', 'Owner', 'Reviewer'])
    sdef.setPermission('Access contents information', 0, ['Anonymous', 'Authenticated', 'Manager', 'Member', 'Owner', 'Reviewer'])
    sdef.setPermission('Change portal events', 0, ['Manager', 'Owner'])

    sdef = wf.states['public']
    sdef.setProperties(title="""""",
                       transitions=('hide', 'retract'))
    sdef.setPermission('Modify portal content', 0, ['Manager', 'Owner'])
    sdef.setPermission('View', 0, ['Authenticated', 'Manager', 'Member', 'Owner', 'Reviewer'])
    sdef.setPermission('Access contents information', 0, ['Anonymous', 'Authenticated', 'Manager', 'Member', 'Owner', 'Reviewer'])
    sdef.setPermission('Change portal events', 0, ['Manager', 'Owner'])


    ## Transitions initialization
    tdef = wf.transitions['make_public']
    tdef.setProperties(title="""Owner""",
                       new_state_id="""public""",
                       trigger_type=1,
                       script_name="""""",
                       after_script_name="""""",
                       actbox_name="""Make public (even for anonymous)""",
                       actbox_url="""""",
                       actbox_category="""workflow""",
                       props={'guard_permissions': 'Review portal content'},
                       )

    tdef = wf.transitions['hide']
    tdef.setProperties(title="""Member makes content private""",
                       new_state_id="""private""",
                       trigger_type=1,
                       script_name="""""",
                       after_script_name="""""",
                       actbox_name="""Make private""",
                       actbox_url="""%(content_url)s/content_hide_form""",
                       actbox_category="""workflow""",
                       props={'guard_roles': 'Owner'},
                       )

    tdef = wf.transitions['publish']
    tdef.setProperties(title="""Reviewer publishes content""",
                       new_state_id="""published""",
                       trigger_type=1,
                       script_name="""""",
                       after_script_name="""moveToArchive""",
                       actbox_name="""Publish (for logged user)""",
                       actbox_url="""%(content_url)s/content_publish_form""",
                       actbox_category="""workflow""",
                       props={'guard_permissions': 'Review portal content'},
                       )

    tdef = wf.transitions['draft']
    tdef.setProperties(title="""Member makes content visible""",
                       new_state_id="""draft""",
                       trigger_type=1,
                       script_name="""""",
                       after_script_name="""""",
                       actbox_name="""Make Visible""",
                       actbox_url="""%(content_url)s/content_show_form""",
                       actbox_category="""workflow""",
                       props={'guard_roles': 'Owner'},
                       )

    tdef = wf.transitions['submit']
    tdef.setProperties(title="""Member requests publishing""",
                       new_state_id="""pending""",
                       trigger_type=1,
                       script_name="""""",
                       after_script_name="""""",
                       actbox_name="""Submit""",
                       actbox_url="""%(content_url)s/content_submit_form""",
                       actbox_category="""workflow""",
                       props={'guard_permissions': 'Request review'},
                       )

    tdef = wf.transitions['reject']
    tdef.setProperties(title="""Reviewer rejects submission""",
                       new_state_id="""draft""",
                       trigger_type=1,
                       script_name="""""",
                       after_script_name="""moveToWeblogRoot""",
                       actbox_name="""Reject""",
                       actbox_url="""%(content_url)s/content_reject_form""",
                       actbox_category="""workflow""",
                       props={'guard_permissions': 'Review portal content'},
                       )

    tdef = wf.transitions['retract']
    tdef.setProperties(title="""Member retracts submission""",
                       new_state_id="""draft""",
                       trigger_type=1,
                       script_name="""""",
                       after_script_name="""""",
                       actbox_name="""Retract""",
                       actbox_url="""%(content_url)s/content_retract_form""",
                       actbox_category="""workflow""",
                       props={'guard_permissions': 'Request review'},
                       )

    tdef = wf.transitions['make_draft']
    tdef.setProperties(title="""Member makes content a draft""",
                       new_state_id="""draft""",
                       trigger_type=1,
                       script_name="""""",
                       after_script_name="""moveToWeblogRoot""",
                       actbox_name="""Revert to Draft""",
                       actbox_url="""""",
                       actbox_category="""workflow""",
                       props={'guard_expr': 'python: user.has_permission("Manage portal content", here) or user.has_role("Owner", here)'},
                       )

    tdef = wf.transitions['publish_in_place']
    tdef.setProperties(title="""Reviewer publishes content without invoking workflow script""",
                       new_state_id="""published""",
                       trigger_type=1,
                       script_name="""""",
                       after_script_name="""""",
                       actbox_name="""""",
                       actbox_url="""""",
                       actbox_category="""""",
                       props={'guard_expr': 'python: user.has_permission("Review portal content", here) or user.has_role("Owner", here)'},
                       )

    ## State Variable
    wf.variables.setStateVar('review_state')

    ## Variables initialization
    vdef = wf.variables['action']
    vdef.setProperties(description="""The last transition""",
                       default_value="""""",
                       default_expr="""transition/getId|nothing""",
                       for_catalog=0,
                       for_status=1,
                       update_always=1,
                       props=None)

    vdef = wf.variables['review_history']
    vdef.setProperties(description="""Provides access to workflow history""",
                       default_value="""""",
                       default_expr="""state_change/getHistory""",
                       for_catalog=0,
                       for_status=0,
                       update_always=0,
                       props={'guard_permissions': 'Request review; Review portal content'})

    vdef = wf.variables['actor']
    vdef.setProperties(description="""The ID of the user who performed the last transition""",
                       default_value="""""",
                       default_expr="""user/getId""",
                       for_catalog=0,
                       for_status=1,
                       update_always=1,
                       props=None)

    vdef = wf.variables['comments']
    vdef.setProperties(description="""Comments about the last transition""",
                       default_value="""""",
                       default_expr="""python:state_change.kwargs.get('comment', '')""",
                       for_catalog=0,
                       for_status=1,
                       update_always=1,
                       props=None)

    vdef = wf.variables['time']
    vdef.setProperties(description="""Time of the last transition""",
                       default_value="""""",
                       default_expr="""state_change/getDateTime""",
                       for_catalog=0,
                       for_status=1,
                       update_always=1,
                       props=None)

    ## Worklists Initialization
    ldef = wf.worklists['reviewer_queue']
    ldef.setProperties(description="""Reviewer tasks""",
                       actbox_name="""Pending (%(count)d)""",
                       actbox_url="""%(portal_url)s/search?review_state=pending""",
                       actbox_category="""global""",
                       props={'guard_permissions': 'Review portal content', 'var_match_review_state': 'pending'})


def createQuills_workflow(id):
    "..."
    ob = DCWorkflowDefinition(id)
    setupQuills_workflow(ob)
    return ob

addWorkflowFactory(createQuills_workflow,
                   id='quills_workflow',
                   title='Quills Workflow (ryzom)')