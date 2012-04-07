function cbaddobstaclecircle(map, eventdata, handles)
%CBADDOBSTACLE Summary of this function goes here
%   Detailed explanation goes here

    pos = get(map, 'CurrentPoint');
    x = pos(1,1); 
    y = pos(1,2);
    handle = imellipse(map, [x y 1 1]);
    setFixedAspectRatioMode(handle, 1);
    
    % Add callback for each obstacle to remove itself when 'remove
    % obstacle' button is selected
    set(handle, 'ButtonDownFcn', {@cbremoveobstacle_, map});
    
    % Add object to list of obstacles
    mapData = get(map, 'UserData');
    mapData.obstacles = [mapData.obstacles handle];
    set(map, 'UserData', mapData);

end

