"""
By Pytel
"""

from enum import Enum
from typing import Union

"""
typedef enum {
    INT,
    FLOAT,
    CHAR
} DATA_TYPE;
"""
class DataType(Enum):
    INT = 0
    FLOAT = 1
    CHAR = 2

    def __str__(self) -> str:
        if self == DataType.INT:
            return 'INT'
        elif self == DataType.FLOAT:
            return 'FLOAT'
        elif self == DataType.CHAR:
            return 'CHAR'
        else:
            return 'UNKNOWN'
        
    def from_int(value: int) -> 'DataType':
        if value == 0:
            return DataType.INT
        elif value == 1:
            return DataType.FLOAT
        elif value == 2:
            return DataType.CHAR
        else:
            return None
    
    def to_size(self) -> str:
        """
        Return size as string for data upnack
        """
        if self == DataType.INT:
            return 'i'
        elif self == DataType.FLOAT:
            return 'f'
        elif self == DataType.CHAR:
            return 's'
        else:
            return 'UNKNOWN'


class DataBlock:
    def __init__(self, data_type: int, data_size: int, data: Union[int, float, bytes]):
        """
        Initialize a DataBlock.

        :param data_type: Type of the data (0: INT, 1: FLOAT, 2: CHAR)
        :param data_size: Size of the data
        :param data: The data itself
        """
        self.data_type = data_type
        self.data_size = data_size
        self.data = data

    def __str__(self) -> str:
        text = "DataBlock:\n"
        # data type
        text += f' - Data type: {DataType(self.data_type)}\n'
        # data size
        text += f' - Data size: {self.data_size}\n'
        # data
        if self.data_type == 0:
            text += f' - Data: {self.data}\n'
        elif self.data_type == 1:
            text += f' - Data: {self.data}\n'
        elif self.data_type == 2:
            text += f' - Data: {self.data.decode()}\n'
        return text

    def to_dict(self) -> dict:
        """
        Convert DataBlock to dictionary.
        """
        return {
            'data_type': self.data_type,
            'data_size': self.data_size,
            'data': self.data if self.data_type != 2 else self.data.decode()
        }
    