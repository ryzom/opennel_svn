/** \file commands.cpp
 * Snowballs 2 specific code for managing the command interface
 *
 * $Id$
 */

/* Copyright, 2001 Nevrax Ltd.
 *
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

void removeAllEntitiesExceptUs ()
{
	EIT eit, nexteit;

	// move entities
	for (eit = Entities.begin (); eit != Entities.end (); )
	{
		nexteit = eit; nexteit++;

		CEntity	&entity = (*eit).second;

		if (entity.Type != CEntity::Self)
		{
			// effectively remove the entity (don't play animation)
			deleteEntity (entity);
		}

		eit = nexteit;
	}
}

	//////// The size of the world, in meter
	//////float WorldWidth = 20 * 160;
	//////float WorldHeight = 6 * 160;


	// these variables are set with the config file

	//////// Setup for the name up the character
	//////float EntityNameSize;
	//////CRGBA EntityNameColor;

// Draw entities names up the characters
void renderEntitiesNames ()
{
	// Setup the driver in matrix mode
	Driver->setMatrixMode3D (Camera);
	// Setup the drawing context
	TextContext->setHotSpot (UTextContext::MiddleTop);
	TextContext->setColor (EntityNameColor);
	TextContext->setFontSize ((uint32)EntityNameSize);
	//
	for (EIT eit = Entities.begin (); eit != Entities.end (); eit++)
	{
		CEntity	&entity = (*eit).second;
		if (!entity.Instance.empty() && entity.Type == CEntity::Other)
		{
			CMatrix		mat = Camera.getMatrix();
			mat.setPos(entity.Position + CVector(0.0f, 0.0f, 4.0f));
			mat.scale(10.0f);
			TextContext->render3D(mat, entity.Name);
		}
	}
}

// The entities preferences callback
void cbUpdateEntities (CConfigFile::CVar &var)
{
	if (var.Name == "EntityNameColor") EntityNameColor.set (var.asInt(0), var.asInt(1), var.asInt(2), var.asInt(3));
	else if (var.Name == "EntityNameSize") EntityNameSize = var.asFloat ();
	else nlwarning ("Unknown variable update %s", var.Name.c_str());
}

void initEntities()
{
	ConfigFile->setCallback ("EntityNameColor", cbUpdateEntities);
	ConfigFile->setCallback ("EntityNameSize", cbUpdateEntities);

	cbUpdateEntities (ConfigFile->getVar ("EntityNameColor"));
	cbUpdateEntities (ConfigFile->getVar ("EntityNameSize"));
}

void releaseEntities()
{
	// Remove config file callbacks
	ConfigFile->setCallback("EntityNameColor", NULL);
	ConfigFile->setCallback("EntityNameSize", NULL);

	// Delete all entities (should already have been called normally)
	deleteAllEntities();
}

//
// Commands
//

NLMISC_COMMAND(remove_entity,"remove a local entity","<eid>")
{
	// check args, if there s not the right number of parameter, return bad
	if(args.size() != 1) return false;

	uint32 eid = (uint32)atoi(args[0].c_str());
	removeEntity (eid);

	return true;
}


NLMISC_COMMAND(add_entity,"add a local entity","<nb_entities> <auto_update>")
{
	// check args, if there s not the right number of parameter, return bad
	if(args.size() != 2) return false;

	uint nb = (uint)atoi(args[0].c_str());

	for (uint i = 0; i < nb ; i++)
	{
		uint32 eid = ++LastEID;
		CVector start(ConfigFile->getVar("StartPoint").asFloat(0), ConfigFile->getVar("StartPoint").asFloat(1), ConfigFile->getVar("StartPoint").asFloat(2));
		addEntity (eid, "Entity"+toString(eid), CEntity::Other, start, start);
		EIT eit = findEntity (eid);
		(*eit).second.AutoMove = atoi(args[1].c_str()) == 1;
	}

	return true;
}

NLMISC_COMMAND(speed,"set the player speed","[<entity_id>] <speed in km/h>")
{
	// check args, if there s not the right number of parameter, return bad
	if(args.size() == 1)
	{
		float speed = min( max( (float)atof(args[0].c_str()), 0.1f ), 200.0f ); // speed range in km/h
		if (Self != NULL)
		{
			MouseListener->setSpeed( speed / 3.6f );
			Self->Speed = speed / 3.6f;
		}
	}
	else if(args.size() == 2)
	{
		float speed = min( max( (float)atof(args[1].c_str()), 0.1f ), 200.0f ); // speed range in km/h

		uint eid = (uint)atoi(args[0].c_str());
		EIT eit = findEntity (eid);
		CEntity	&entity = (*eit).second;

		entity.Speed = speed / 3.6f;
		if (entity.Type == CEntity::Self)
		{
			MouseListener->setSpeed(entity.Speed);
		}
	}
	return true;
}

NLMISC_COMMAND(goto, "go to a given position", "<x> <y>")
{
	// check args, if there s not the right number of parameter, return bad
	if(args.size() != 2) return false;

	if (Self == NULL) return false;

	CEntity	&entity = *Self;

	float	x, y;

	x = (float)atof(args[0].c_str());
	y = (float)atof(args[1].c_str());

	//
	if (entity.MovePrimitive != NULL && entity.VisualCollisionEntity != NULL)
	{
		UGlobalPosition	gPos;
		entity.MovePrimitive->setGlobalPosition(CVectorD(x, y, 0.0f), 0);
		// get the global position in pacs coordinates system
		entity.MovePrimitive->getGlobalPosition(gPos, 0);
		// convert it in a vector 3d
		entity.Position = GlobalRetriever->getGlobalPosition(gPos);
		// get the good z position
		gPos.LocalPosition.Estimation.z = 0.0f;
		entity.Position.z = GlobalRetriever->getMeanHeight(gPos);
		// snap to the ground
		if (!GlobalRetriever->isInterior(gPos))
		   entity.VisualCollisionEntity->snapToGround( entity.Position );

		if (entity.Type == CEntity::Self)
		{
			MouseListener->setPosition(entity.Position);
		}
	}

	return true;
}

NLMISC_COMMAND(entities, "display all entities info", "")
{
	// check args, if there s not the right number of parameter, return bad
	if(args.size() != 0) return false;

	for (EIT eit = Entities.begin (); eit != Entities.end (); eit++)
	{
		CEntity	&e = (*eit).second;
		log.displayNL("%s %u (k%u) %s %d", (Self==&e)?"*":" ", e.Id, (*eit).first, e.Name.c_str(), e.Type);
	}
	return true;
}

NLMISC_COMMAND(test_cls, "test the collision service, disables collision test on self", "")
{
	_TestCLS = !_TestCLS;
	return true;
}
