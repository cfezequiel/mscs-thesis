function cbremovewaypoint(map, eventdata, handles)
%CBREMOVEWAYPOINT Summary of this function goes here
%   Detailed explanation goes here

    mapData = get(map, 'UserData');
    mapData.state = 'Remove Waypoint';
    set(map, 'UserData', mapData);    
end

