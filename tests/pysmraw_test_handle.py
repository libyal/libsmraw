#!/usr/bin/env python
#
# Python-bindings handle type test script
#
# Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.

import argparse
import os
import sys
import unittest

import pysmraw


class HandleTypeTests(unittest.TestCase):
  """Tests the handle type."""

  def test_signal_abort(self):
    """Tests the signal_abort function."""
    smraw_handle = pysmraw.handle()

    smraw_handle.signal_abort()

  def test_open(self):
    """Tests the open function."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    smraw_handle = pysmraw.handle()
    filenames = pysmraw.glob(unittest.source)

    smraw_handle.open(filenames)

    with self.assertRaises(IOError):
      smraw_handle.open(filenames)

    smraw_handle.close()

    with self.assertRaises(TypeError):
      smraw_handle.open(None)

    # TODO: add open write test.

  def test_open_file_objects(self):
    """Tests the open_file_objects function."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    smraw_handle = pysmraw.handle()

    filenames = pysmraw.glob(unittest.source)
    file_objects = []
    for filename in filenames:
      file_object = open(filename, "rb")
      file_objects.append(file_object)

    smraw_handle.open_file_objects(file_objects)

    with self.assertRaises(IOError):
      smraw_handle.open_file_objects(file_objects)

    smraw_handle.close()

    # TODO: change IOError into TypeError
    with self.assertRaises(IOError):
      smraw_handle.open_file_objects(None)

    with self.assertRaises(ValueError):
      smraw_handle.open_file_objects(file_objects, mode="w")

  def test_close(self):
    """Tests the close function."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    smraw_handle = pysmraw.handle()

    with self.assertRaises(IOError):
      smraw_handle.close()

  def test_open_close(self):
    """Tests the open and close functions."""
    if not unittest.source:
      return

    smraw_handle = pysmraw.handle()
    filenames = pysmraw.glob(unittest.source)

    # Test open and close.
    smraw_handle.open(filenames)
    smraw_handle.close()

    # Test open and close a second time to validate clean up on close.
    smraw_handle.open(filenames)
    smraw_handle.close()

    file_object = open(unittest.source, "rb")

    # Test open_file_objects and close.
    smraw_handle.open_file_objects([file_object])
    smraw_handle.close()

    # Test open_file_objects and close a second time to validate clean up on close.
    smraw_handle.open_file_objects([file_object])
    smraw_handle.close()

    # Test open_file_objects and close and dereferencing file_object.
    smraw_handle.open_file_objects([file_object])
    del file_object
    smraw_handle.close()

  def test_read_buffer(self):
    """Tests the read_buffer function."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    smraw_handle = pysmraw.handle()
    filenames = pysmraw.glob(unittest.source)

    smraw_handle.open(filenames)

    media_size = smraw_handle.get_media_size()

    # Test read with maximum size.
    data = smraw_handle.read_buffer(size=4096)

    self.assertIsNotNone(data)
    self.assertEqual(len(data), min(media_size, 4096))

    # Test read with maximum size beyond file size.
    if media_size > 16:
      smraw_handle.seek_offset(-16, os.SEEK_END)

      data = smraw_handle.read_buffer(size=4096)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), 16)

    # Test read without maximum size.
    if media_size < 4096:
      smraw_handle.seek_offset(0, os.SEEK_SET)

      data = smraw_handle.read_buffer()

      self.assertIsNotNone(data)
      self.assertEqual(len(data), media_size)

    with self.assertRaises(ValueError):
      smraw_handle.read_buffer(size=-1)

    smraw_handle.close()

    # Test the read without open.
    with self.assertRaises(IOError):
      smraw_handle.read_buffer(size=4096)

  def test_read_buffer_file_objects(self):
    """Tests the read_buffer function on file-like objects."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    file_object = open(unittest.source, "rb")

    smraw_handle = pysmraw.handle()

    smraw_handle.open_file_objects([file_object])

    media_size = smraw_handle.get_media_size()

    # Test normal read.
    data = smraw_handle.read_buffer(size=4096)

    self.assertIsNotNone(data)
    self.assertEqual(len(data), min(media_size, 4096))

    smraw_handle.close()

  def test_read_buffer_at_offset(self):
    """Tests the read_buffer_at_offset function."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    smraw_handle = pysmraw.handle()
    filenames = pysmraw.glob(unittest.source)

    smraw_handle.open(filenames)

    media_size = smraw_handle.get_media_size()

    # Test normal read.
    data = smraw_handle.read_buffer_at_offset(4096, 0)

    self.assertIsNotNone(data)
    self.assertEqual(len(data), min(media_size, 4096))

    # Test read beyond file size.
    if media_size > 16:
      data = smraw_handle.read_buffer_at_offset(4096, media_size - 16)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), 16)

    with self.assertRaises(ValueError):
      smraw_handle.read_buffer_at_offset(-1, 0)

    with self.assertRaises(ValueError):
      smraw_handle.read_buffer_at_offset(4096, -1)

    smraw_handle.close()

    # Test the read without open.
    with self.assertRaises(IOError):
      smraw_handle.read_buffer_at_offset(4096, 0)

  def test_seek_offset(self):
    """Tests the seek_offset function."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    smraw_handle = pysmraw.handle()
    filenames = pysmraw.glob(unittest.source)

    smraw_handle.open(filenames)

    media_size = smraw_handle.get_media_size()

    smraw_handle.seek_offset(16, os.SEEK_SET)

    offset = smraw_handle.get_offset()
    self.assertEqual(offset, 16)

    smraw_handle.seek_offset(16, os.SEEK_CUR)

    offset = smraw_handle.get_offset()
    self.assertEqual(offset, 32)

    smraw_handle.seek_offset(-16, os.SEEK_CUR)

    offset = smraw_handle.get_offset()
    self.assertEqual(offset, 16)

    if media_size > 16:
      smraw_handle.seek_offset(-16, os.SEEK_END)

      offset = smraw_handle.get_offset()
      self.assertEqual(offset, media_size - 16)

    smraw_handle.seek_offset(16, os.SEEK_END)

    offset = smraw_handle.get_offset()
    self.assertEqual(offset, media_size + 16)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      smraw_handle.seek_offset(-1, os.SEEK_SET)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      smraw_handle.seek_offset(-32 - media_size, os.SEEK_CUR)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      smraw_handle.seek_offset(-32 - media_size, os.SEEK_END)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      smraw_handle.seek_offset(0, -1)

    smraw_handle.close()

    # Test the seek without open.
    with self.assertRaises(IOError):
      smraw_handle.seek_offset(16, os.SEEK_SET)

  def test_get_offset(self):
    """Tests the get_offset function and offset property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    smraw_handle = pysmraw.handle()
    filenames = pysmraw.glob(unittest.source)
    smraw_handle.open(filenames)

    offset = smraw_handle.get_offset()
    self.assertIsNotNone(offset)

    smraw_handle.close()

  def test_get_media_size(self):
    """Tests the get_media_size function and media_size property."""
    if not unittest.source:
      raise unittest.SkipTest("missing source")

    smraw_handle = pysmraw.handle()
    filenames = pysmraw.glob(unittest.source)
    smraw_handle.open(filenames)

    media_size = smraw_handle.get_media_size()
    self.assertIsNotNone(media_size)

    self.assertIsNotNone(smraw_handle.media_size)

    smraw_handle.close()


if __name__ == "__main__":
  argument_parser = argparse.ArgumentParser()

  argument_parser.add_argument(
      "source", nargs="?", action="store", metavar="PATH",
      default=None, help="path of the source file.")

  options, unknown_options = argument_parser.parse_known_args()
  unknown_options.insert(0, sys.argv[0])

  setattr(unittest, "source", options.source)

  unittest.main(argv=unknown_options, verbosity=2)
