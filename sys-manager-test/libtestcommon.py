#!/usr/bin/env python
# -*- coding: utf-8 -*-

class Command:
	def __init__(self):
		self.text = ''
		self.idid = ''
		self.description = ''

class WorkFlow:
	def __init__(self):
		self.cmd_id_list = []

class Case:
	def __init__(self):
		self.cmd_list = []
		self.workflow = WorkFlow()
