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

    % Add callback for each waypoint to remove itself when 'remove
    % waypoint' button is selected
    set(h, 'ButtonDownFcn', {@cbremovewaypoint_, map});  
    
    % Update map data
    mapData = get(map, 'UserData');
    mapData.waypoints = [mapData.waypoints h];
    set(map, 'UserData', mapData);

end

