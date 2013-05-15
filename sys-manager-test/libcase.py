#!/usr/bin/env python
# -*- coding: utf-8 -*-

import commands
import xml
import sys
from xml.dom import minidom
from libtestcommon import Command
from libtestcommon import WorkFlow
from libtestcommon import Case


# ----------------------------------------
# xml操作
# ----------------------------------------

def __load_xml(filename):
	try:
		doc = minidom.parse(filename)
	except IOError,e:
		print e
		return False
	except xml.parsers.expat.ExpatError, e:
		print e
		return False
	except:
		print 'Unknow error'
		return False
	return doc

def __get_xmlnode(node, name):
	return node.getElementsByTagName(name) if node else None

def __add_xmlnode(root, name):
	_impl = minidom.getDOMImplementation()
	_dom = _impl.createDocument(None, name, None)
	_node = _dom.createElement(name)
	root.appendChild(_node)
	return _node

def __get_attrvalue(node, attrname):
	return node.getAttribute(attrname) if node else None

def __set_attrvalue(node, attr, value):
	return node.setAttribute(attr, value)

def __remove_attr(node, attr):
	return node.removeAttribute(attr)

def __get_nodevalue(node):
	return node[0].childNodes[0].toxml()

def CmdLoad(nodes):
	_cmd_list = []
	for x in nodes:
		_cmd = Command()
		_cmd.idid = __get_attrvalue(x, 'id')
		_cmd.text = __get_nodevalue(__get_xmlnode(x, 'text'))
		_cmd.description = __get_nodevalue(__get_xmlnode(x, 'description'))
		_cmd_list.append(_cmd)
	return _cmd_list

def WorkFlowLoad(nodes):
	_cmd_id_list = []
	for x in __get_xmlnode(nodes[0], 'step'):
		_cmd_id_list.append(__get_attrvalue(x, 'cmd_id'))
	return _cmd_id_list

def CaseLoad(fname):
	doc = __load_xml(fname)
	if doc == False:
		return
	root = doc.documentElement

	_case = Case()
	_case.name = fname
	_case.cmd_list = CmdLoad(__get_xmlnode(root, 'command'))
	_case.workflow.cmd_id_list = WorkFlowLoad(__get_xmlnode(root, 'work_flow'))

	return _case

def __self_test():
	_case = CaseLoad(sys.argv[1])

	print '-------- command list ---------'
	for x in _case.cmd_list:
		print 'ID: ', x.idid
		print 'CMD: ', x.text
		print 'DESC: ', x.description

	print '--------- work flow ----------'
	for x in _case.workflow.cmd_id_list:
		print 'STEP: ', x

if __name__ == '__main__':
	__self_test()
