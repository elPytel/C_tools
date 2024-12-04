classdef Message
    properties
        number_of_bytes
        number_of_sections
        msg_type
        data_blocks
    end
    
    methods
        function obj = Message(number_of_bytes, number_of_sections, msg_type, data_blocks)
            obj.number_of_bytes = number_of_bytes;
            obj.number_of_sections = number_of_sections;
            obj.msg_type = msg_type;
            obj.data_blocks = data_blocks;
        end
        
        function str = toString(obj)
            str = sprintf('Message:\n - Number of bytes: %d\n - Number of sections: %d\n - Message type: %s\n', ...
                obj.number_of_bytes, obj.number_of_sections, char(obj.msg_type));
            str = strcat(str, 'Data blocks:\n');
            for i = 1:obj.number_of_sections
                str = strcat(str, sprintf('Section %d:\n', i-1));
                str = strcat(str, obj.data_blocks{i}.toString());
            end
        end
        
        function jsonStr = toJson(obj)
            data_blocks = cell(1, obj.number_of_sections);
            for i = 1:obj.number_of_sections
                data_blocks{i} = struct('data_type', char(obj.data_blocks{i}.data_type), ...
                    'data_size', obj.data_blocks{i}.data_size, ...
                    'data', obj.data_blocks{i}.data);
            end
            msgStruct = struct('number_of_bytes', obj.number_of_bytes, ...
                'number_of_sections', obj.number_of_sections, ...
                'msg_type', char(obj.msg_type), ...
                'data_blocks', {data_blocks});
            jsonStr = jsonencode(msgStruct);
        end
    end
    
    methods (Static)
        function obj = fromBinary(file_name)
            fid = fopen(file_name, 'rb');
            buffer = fread(fid, '*uint8');
            fclose(fid);
            
            offset = 1;
            number_of_bytes = typecast(buffer(offset:offset+3), 'uint32');
            offset = offset + 4;
            number_of_sections = buffer(offset);
            offset = offset + 1;
            msg_type = typecast(buffer(offset:offset+3), 'uint32');
            offset = offset + 4;
            
            data_blocks = cell(1, number_of_sections);
            for i = 1:number_of_sections
                data_type = typecast(buffer(offset:offset+3), 'uint32');
                offset = offset + 4;
                data_size = typecast(buffer(offset:offset+1), 'uint16');
                offset = offset + 2;
                if data_type == 0
                    data = typecast(buffer(offset:offset+3), 'int32');
                    offset = offset + 4;
                elseif data_type == 1
                    data = typecast(buffer(offset:offset+3), 'single');
                    offset = offset + 4;
                elseif data_type == 2
                    data = char(buffer(offset:offset+data_size-1))';
                    offset = offset + data_size;
                end
                data_blocks{i} = DataBlock(DataType(data_type + 1), data_size, data); % Adjusted conversion
            end
            
            obj = Message(number_of_bytes, number_of_sections, MessageType(msg_type), data_blocks);
        end
    end
end