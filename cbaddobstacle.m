function cbaddobstacle(map, eventdata, handles)
%CBADDOBSTACLE Summary of this function goes here
%   Detailed explanation goes here

    pos = get(map, 'CurrentPoint');
    x = pos(1,1); 
    y = pos(1,2);
    handle = line(x, y, 0, 'Marker', 'o');
    % Add callback for each obstacle to remove itself when 'remove
    % obstacle' button is selected
    set(handle, 'ButtonDownFcn', {@cbremoveobstacle_, map});  
    mapData = get(map, 'UserData');
    mapData.obstacles = [mapData.obstacles handle];
    set(map, 'UserData', mapData);

end

