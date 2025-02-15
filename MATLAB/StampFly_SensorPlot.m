%List M5 Stack in BLE
list = blelist("Name","StampFly_BLE");

%Connect to the device
m5 = ble(list.Address);
m5.Characteristics

ServiceUUID = "1A8E4E30-DEF4-4D96-A1F4-7E6101B7EEA4";
characteristicUID = "8F9A9BC0-2BDD-40DD-8BB5-CB9D4E3799FF";  % For interacting data to/from StampFly

% Get the characteristic for writing (send data)
data_c = characteristic(m5, ServiceUUID, characteristicUID);

%% Example to send data to StampFly
d_RUDDER = 0;
d_THROTTLE = 0;
d_AILERON = 0;
d_ELEVATOR =0;
d_BUTTON_ARM = 0;
d_BUTTON_FLIP = 0;
d_CONTROLMODE = 0;
d_ALTCONTROLMODE = 0;

% Convert to byte array
valuesToSend = typecast(single([d_RUDDER, ...
                               d_THROTTLE, ...
                               d_AILERON, ...
                               d_ELEVATOR, ...
                               d_BUTTON_ARM, ...
                               d_BUTTON_FLIP, ...
                               d_CONTROLMODE, ...
                               d_ALTCONTROLMODE]), 'uint8');



% Send data to Stampfly
write(data_c, valuesToSend);

%% 

disp('Press Ctrl+C to stop.');

% Open a plot figure window. 
ax = poseplot(ones("quaternion"),[0 0 0],MeshFileName="multirotor.stl",ScaleFactor=0.5);

% Main loop (polling for data)
while true
    try
        % Read raw BLE data (88 bytes expected)
        
        rawData = read(data_c);

        if length(rawData) == 84
            % Convert raw bytes to float values
            %elapsed_time  = typecast(uint8(rawData(1:4)), 'single');
            %interval_time = typecast(uint8(rawData(5:8)), 'single');

            roll_angle     = typecast(uint8(rawData(9:12)), 'single');
            pitch_angle    = typecast(uint8(rawData(13:16)), 'single');
            yaw_angle      = typecast(uint8(rawData(17:20)), 'single');

            %roll_rate     = typecast(uint8(rawData(21:24)), 'single');
            %pitch_rate    = typecast(uint8(rawData(25:28)), 'single');
            %yaw_rate      = typecast(uint8(rawData(29:32)), 'single');
            %accx          = typecast(uint8(rawData(33:36)), 'single');
            %accy          = typecast(uint8(rawData(37:40)), 'single');
            %accz          = typecast(uint8(rawData(41:44)), 'single');
            %Alt_velocity  = typecast(uint8(rawData(45:48)), 'single');
            %Z_dot_ref     = typecast(uint8(rawData(49:52)), 'single');
            %Alt_ref       = typecast(uint8(rawData(53:56)), 'single');
            %Altitude2     = typecast(uint8(rawData(57:60)), 'single');
            %Altitude      = typecast(uint8(rawData(61:64)), 'single');
            %Az            = typecast(uint8(rawData(65:68)), 'single');
            %Az_bias       = typecast(uint8(rawData(69:72)), 'single');
            %Alt_flag      = typecast(uint8(rawData(73:76)), 'single');
            %Mode          = typecast(uint8(rawData(77:80)), 'single');
            %RangeFront    = typecast(uint8(rawData(81:84)), 'single');
            %Voltage        = typecast(uint8(rawData(85:88)), 'single');
 
             q = quaternion([roll_angle pitch_angle yaw_angle],"eulerd", "XYZ","point");
            ax.Orientation = q;

        end
    catch ME
        disp(['Error reading data: ', ME.message]);
    end

    pause(1/400);  % Poll every 1/400 second
end