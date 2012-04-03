function cbaddwaypoint(map, eventdata, handles)
%ADDWAYPOINT Summary of this function goes here
%   Detailed explanation goes here

    % Get current mouse position
    pos = get(map, 'CurrentPoint');
    x = pos(1,1); 
    y = pos(1,2);
    
    % Add waypoint to map
    handle = line(x, y, 0, 'Marker', 'x');
    
    % Add callback for each waypoint to remove itself when 'remove
    % waypoint' button is selected
    set(handle, 'ButtonDownFcn', {@cbremovewaypoint_, map});  
    
    % Update map data
    mapData = get(map, 'UserData');
    mapData.waypoints = [mapData.waypoints handle]
    set(map, 'UserData', mapData);

end

