classdef DataBlock
    properties
        data_type
        data_size
        data
    end
    
    methods
        function obj = DataBlock(data_type, data_size, data)
            obj.data_type = data_type;
            obj.data_size = data_size;
            obj.data = data;
        end
        
        function str = toString(obj)
            str = sprintf('DataBlock:\n - Data type: %s\n - Data size: %d\n', ...
                char(obj.data_type), obj.data_size);
            if obj.data_type == DataType.INT
                str = strcat(str, sprintf(' - Data: %d\n', obj.data));
            elseif obj.data_type == DataType.FLOAT
                str = strcat(str, sprintf(' - Data: %f\n', obj.data));
            elseif obj.data_type == DataType.CHAR
                str = strcat(str, sprintf(' - Data: %s\n', char(obj.data)));
            end
        end
    end
end