function cbaddwaypoint(map, eventdata, handles)
%ADDWAYPOINT Summary of this function goes here
%   Detailed explanation goes here

    % Get current mouse position
    pos = get(map, 'CurrentPoint');
    x = pos(1,1); 
    y = pos(1,2);
    
    % Add waypoint to map
    h = impoint(map, x, y);
    % Construct boundary constraint function
    fcn = makeConstrainToRectFcn('impoint', get(map,'XLim'), get(map,'YLim'));
    % Enforce boundary constraint function using setPositionConstraintFcn
    setPositionConstraintFcn(h, fcn);
    
    % Construct boundary constraint function
    fcn = makeConstrainToRectFcn('impoint',get(gca,'XLim'),get(gca,'YLim'));
    % Enforce boundary constraint function using setPositionConstraintFcn
    setPositionConstraintFcn(h,fcn);

    % Add delete callback, which is activated when 'delete' tool selected
    set(h, 'ButtonDownFcn', {@cbdeletewaypoint, map, handles.lbWaypoints});  

    % Set waypoint name
    mapInfo = get(map, 'UserData');
    % NOTE: 26 - Number of lowercase letters 
    maxChars = 26; 
    offset = mod(mapInfo.nextWaypointIndex, maxChars);
    reps = ((mapInfo.nextWaypointIndex - offset) / maxChars) + 1;
    % NOTE: 97 - ASCII integer for 'a'
    iChar = 97 + offset;
    name = char(ones(1, reps) .* iChar); 
    setString(h, name);
    set(h, 'DisplayName', name);

    % Set waypoint priority
    set(h, 'UserData', mapInfo.nextWaypointIndex);
    
    % Add waypoint to listbox
    list = get(handles.lbWaypoints, 'String');
    list{end + 1} = name;
    set(handles.lbWaypoints, 'String', list);
    
    % Update map data
    mapInfo.waypoints = [mapInfo.waypoints; h];
    mapInfo.nextWaypointIndex = mapInfo.nextWaypointIndex + 1;
    set(map, 'UserData', mapInfo);

end

