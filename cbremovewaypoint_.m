function [ output_args ] = cbremovewaypoint_(src, event, map)
%CBREMOVEWAYPOINT_ Summary of this function goes here
%   Detailed explanation goes here

    mapData = get(map, 'UserData');
    if strcmp(mapData.state, 'Remove Waypoint') 
        i = find(mapData.waypoints == src);
        mapData.waypoints(i) = [];
        delete(src);
        set(map, 'UserData', mapData);
end

