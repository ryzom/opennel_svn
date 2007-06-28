# -*- coding: utf-8 -*-
## PloneSubscription
## A Plone tool supporting different levels of subscription and notification
## Copyright (C)2006 Ingeniweb

## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.

## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with this program; see the file COPYING. If not, write to the
## Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

# parsetab.py
# This file is automatically generated. Do not edit.

_lr_method = 'SLR'

_lr_signature = '{\xb7\xb0my\xdcr\t4\x06\x1b0\xf4~\xb0!'

_lr_action_items = {'AND':([18,26,14,11,16,1,27,24,28,8,23,7,9,15,22,0,6,21,5,2,13,20,10,4,12,19,3,],[-19,-14,1,-15,1,-16,-10,12,-1,-6,12,-13,1,1,12,1,-18,12,-17,-9,12,-6,1,1,1,12,12,]),'CLOSEP':([11,18,26,2,28,1,22,24,8,23,6,9,15,14,21,5,13,20,12,19,27,],[-15,-19,-14,-9,-1,-16,-2,-3,-6,-4,-18,-19,-17,-18,28,-17,-5,-6,-16,-7,-10,]),'$':([8,23,9,1,24,18,26,12,19,3,27,13,11,28,14,2,5,15,22,6,20,],[-6,-4,-19,-16,-3,-19,-14,-16,-7,0,-10,-5,-15,-1,-18,-9,-17,-17,-2,-18,-6,]),'QUOTE':([0,23,12,8,9,1,14,24,2,25,18,22,17,19,3,27,13,6,10,20,28,16,11,21,5,15,26,],[7,7,7,-6,7,-16,7,7,-9,-11,-19,7,26,7,7,-10,7,-18,7,-6,-1,-12,-15,7,-17,7,-14,]),'OPENP':([27,3,19,12,20,13,5,14,6,22,15,28,23,0,21,8,1,9,2,10,26,24,18,11,],[-10,10,10,10,-6,10,-17,10,-18,10,10,-1,10,10,10,-6,-16,10,-9,10,-14,10,-19,-15,]),'NEAR':([1,16,24,23,9,7,18,26,2,10,20,13,28,4,19,12,27,3,15,11,22,0,6,14,21,8,5,],[-16,6,14,14,6,-13,-19,-14,-9,6,-6,14,-1,6,14,6,-10,14,6,-15,14,6,-18,6,14,-6,-17,]),'NOT':([18,10,26,9,1,0,16,24,23,11,8,7,22,15,2,27,6,14,21,5,20,13,4,19,12,28,3,],[-19,9,-14,9,-16,9,18,9,9,-15,-6,-13,9,9,-9,-10,-18,9,9,-17,-6,9,18,9,9,-1,9,]),'OR':([1,16,9,24,20,23,0,8,13,7,18,11,26,2,10,28,4,19,12,27,22,3,15,6,21,14,5,],[-16,5,5,15,-6,15,5,-6,15,-13,-19,-15,-14,-9,5,-1,5,15,5,-10,15,15,5,-18,15,5,-17,]),'STRING':([6,15,22,5,14,4,21,13,20,3,27,12,19,26,18,11,10,2,24,28,9,16,1,7,8,23,0,],[-18,11,11,-17,11,11,11,11,-6,11,-10,11,11,-14,-19,-15,11,-9,11,-1,11,11,-16,-13,-6,11,11,]),}

_lr_action = { }
for _k, _v in _lr_action_items.items():
   for _x,_y in zip(_v[0],_v[1]):
       _lr_action[(_x,_k)] = _y
del _lr_action_items

_lr_goto_items = {'string':([4,24,9,16,15,22,3,10,12,21,13,14,19,23,0,],[16,2,2,16,2,2,2,2,2,2,2,2,2,2,2,]),'expr':([13,23,9,19,15,24,12,10,0,14,21,22,3,],[13,13,19,13,24,13,22,21,3,23,13,13,13,]),'quoteend':([17,],[27,]),'qterm':([16,4,],[25,17,]),'quotestart':([24,3,19,14,21,0,23,10,12,15,9,13,22,],[4,4,4,4,4,4,4,4,4,4,4,4,4,]),'factor':([14,0,21,10,12,23,3,15,22,9,13,24,19,],[8,8,8,8,8,8,8,8,8,20,8,8,8,]),}

_lr_goto = { }
for _k, _v in _lr_goto_items.items():
   for _x,_y in zip(_v[0],_v[1]):
       _lr_goto[(_x,_k)] = _y
del _lr_goto_items
