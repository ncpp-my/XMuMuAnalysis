import collections

class MyDict(collections.OrderedDict):
  def __missing__(self, key):
    val = self[key] = MyDict()
    return val
