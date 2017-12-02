"""
each datapoint contains:
- timestamp when it was stored
- actual value
- valid flag
"""

class DataPoint:
    def __init__(self, timestamp=-1, value=None):
        self.timestamp = timestamp
        self.value = value
        self.valid = False
        if timestamp != -1 and value is not None:
            self.valid = True

    def getValue(self):
        """
        returns the stored value.
        """
        if not self.valid:
            return None
        return value
        
"""
list of data

has some nice features like cleaning up old data
and returning media or median values for stored 
data. default type can be:
    0 = mean value
    1 = median value
    2 = newest value 
"""
class DataList:
    def __init__(self, type=0):
        self._data = {}
        self.dataType = type
    
    def addData(self, data):
        """
        add data to local representation
        if valid
        """
        if not data.valid:
            print "dropping data since not valid"
            return
        self._data[data.timestamp] = data.value

    def __iter__(self):
        for key in sorted(self._data.keys()):
            yield self._data[key]

    def cleanup(self, timestamp):
        """ 
        removes values older than timestamp
        """
        for k in self._data.keys():
            if k < timestamp:
                del self._data[k]
    
    def get(self):
        """
        return value for stored values
        """
        if not len(self._data):
            return 0.0

        if self.dataType == 1:
            return self.median()
        elif self.dataType == 2:
            return self._data[sorted(self._data.keys())[-1]]

        return self.mean()

    def mean(self):
        """
        gets mean value for list
        """
        if not len(self._data):
            return 0.0

        v = sum(self._data.values())
        return v / float(len(self._data))

    def median(self):
        """
        gets median value for list
        """
        if not len(self._data):
            return 0.0

        if len(self._data) == 1:
            return self._data.values()[0]
    
        p = int(len(self._data)/2)
        return sorted(self._data.values())[p]
    
if __name__ == "__main__":
    print "create fake data"

    data = DataList()

    data.addData(DataPoint())
    data.addData(DataPoint(1,1.0))
    data.addData(DataPoint(2,1.1))
    data.addData(DataPoint(4,1.3))
    data.addData(DataPoint(3,1.4))
    data.addData(DataPoint(0,0.0))
    for d in data:
        print d
    print(data.mean())
    print(data.median())

    print("cleanup")
    data.cleanup(2)
    for d in data:
        print d
    print(data.mean())
    print(data.median())
