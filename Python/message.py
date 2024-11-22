"""
By Pytel
"""

import struct
from typing import List
from enum import Enum
import json

from datablock import DataBlock, DataType

"""
typedef enum {
    LOGING_INFO,
    LOGING_WARNING,
    LOGING_ERROR
} MSG_TYPE;
"""
class MessageType(Enum):
    LOGING_INFO = 0
    LOGING_WARNING = 1
    LOGING_ERROR = 2

    def __str__(self) -> str:
        if self == MessageType.LOGING_INFO:
            return 'LOGING_INFO'
        elif self == MessageType.LOGING_WARNING:
            return 'LOGING_WARNING'
        elif self == MessageType.LOGING_ERROR:
            return 'LOGING_ERROR'
        else:
            return 'UNKNOWN'

    def from_int(value: int) -> 'MessageType':
        if value == 0:
            return MessageType.LOGING_INFO
        elif value == 1:
            return MessageType.LOGING_WARNING
        elif value == 2:
            return MessageType.LOGING_ERROR
        else:
            return None

    def to_int(self) -> int:
        if self == MessageType.LOGING_INFO:
            return 0
        elif self == MessageType.LOGING_WARNING:
            return 1
        elif self == MessageType.LOGING_ERROR:
            return 2
        else:
            return -1

class Message:
    def __init__(self, number_of_bytes: int, number_of_sections: int, msg_type: int, data_blocks: List[DataBlock]):
        """
        Initialize a Message.

        :param number_of_bytes: Total number of bytes in the message
        :param number_of_sections: Number of data sections in the message
        :param msg_type: Type of the message
        :param data_blocks: List of data blocks
        """
        self.number_of_bytes = number_of_bytes
        self.number_of_sections = number_of_sections
        self.msg_type = msg_type
        self.data_blocks = data_blocks

    @staticmethod
    def from_binary(file_name: str) -> 'Message':
        """
        Create a Message object from a binary file.

        :param file_name: Path to the binary file
        :return: Message object
        """
        with open(file_name, 'rb') as file:
            buffer = file.read()
        
        offset = 0
        number_of_bytes = struct.unpack_from('I', buffer, offset)[0]
        offset += 4
        number_of_sections = struct.unpack_from('B', buffer, offset)[0]
        offset += 1
        msg_type = struct.unpack_from('I', buffer, offset)[0]
        offset += 4

        data_blocks = []
        for _ in range(number_of_sections):
            data_type = struct.unpack_from('I', buffer, offset)[0]
            offset += 4
            data_size = struct.unpack_from('H', buffer, offset)[0]
            offset += 2
            data_format = DataType.from_int(data_type).to_size()
            data = struct.unpack_from(data_format, buffer, offset)[0]
            offset += struct.calcsize(data_format)
            data_blocks.append(DataBlock(data_type, data_size, data))

        return Message(number_of_bytes, number_of_sections, msg_type, data_blocks)
    
    def __str__(self) -> str:
        text = 'Message:\n'
        text += f' - Number of bytes: {self.number_of_bytes}\n'
        text += f' - Number of sections: {self.number_of_sections}\n'
        text += f' - Message type: {self.msg_type}\n'
        text += 'Data blocks:\n'
        for i, data_block in enumerate(self.data_blocks):
            text += f'Section {i}:\n'
            text += str(data_block)
        return text

    def to_dict(self) -> dict:
        """
        Convert Message to dictionary.
        """
        return {
            'number_of_bytes': self.number_of_bytes,
            'number_of_sections': self.number_of_sections,
            'msg_type': self.msg_type,
            'data_blocks': [data_block.to_dict() for data_block in self.data_blocks]
        }

    def to_json(self) -> str:
        """
        Convert Message to JSON string.
        """
        return json.dumps(self.to_dict(), indent=4)

if __name__ == '__main__':
    msg = Message.from_binary('test_message.bin')
    print(msg)
    print(msg.to_json())