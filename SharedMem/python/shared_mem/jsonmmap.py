#!/usr/bin/python
# -*- coding: utf-8 -*-
import mmap
import json
# from mmap import mmap


class ObjectMmap(mmap.mmap):
  def __init__(self, fileno=-1, length=1024, access=mmap.ACCESS_WRITE, tagname='share_mmap'):
      # super(ObjectMmap, self).__init__(self, fileno, length, access=access, tagname=tagname)
      self.mm = mmap.mmap(fileno, length, access=access, tagname=tagname)
      self.length = length
      self.access = access
      self.tagname = tagname

  def jsonwrite(self, obj):
      try:
          self.obj = obj
          self.mm.seek(0)
          obj_str = json.dumps(obj)
          obj_len = len(obj_str)
          content = str(obj_len) + ":" + obj_str
          self.mm.write(content.encode())
          self.contentbegin = len(str(obj_len)) + 1
          self.contentend = self.mm.tell()
          self.contentlength = self.contentend - self.contentbegin
          return True
      except:
          return False

  def jsonread_master(self):
      try:
          self.mm.seek(self.contentbegin)
          content = self.mm.read(self.contentlength)
          obj = json.loads(content)
          self.obj = obj
          return obj
      except:
          if self.obj:
              return self.obj
          else:
              return None

  def jsonread_follower(self):
      # try:
          self.mm.seek(0)
          index = self.mm.find(":".encode())
          if index != -1:
              head = self.mm.read(index + 1)
              contentlength = int(head[:-1])
              content = self.mm.read(contentlength)
              obj = json.loads(content)
              self.obj = obj
              return obj
          else:
              return None
      # except:
      #     if self.obj:
      #         return self.obj
      #     else:
      #         return None