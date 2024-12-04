% Print all enums with their values:
disp('DataType enumeration values:');
data_types = enumeration('DataType');
for i = 1:numel(data_types)
    fprintf('%s = %d\n', char(data_types(i)), int32(data_types(i)));
end

disp('MessageType enumeration values:');
message_types = enumeration('MessageType');
for i = 1:numel(message_types)
    fprintf('%s = %d\n', char(message_types(i)), int32(message_types(i)));
end

% Example usage
msg = Message.fromBinary('test_message.bin');
disp(msg.toString());
disp(msg.toJson());