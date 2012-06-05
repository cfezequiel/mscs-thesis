function cbaddobstacleline(map, eventdata, handles)
%CBADDOBSTACLELINE Add a draggable, resizable line obstacle
%   Detailed explanation goes here

    pos = get(map, 'CurrentPoint');
    x = pos(1,1); 
    y = pos(1,2);
    h = imline(map, [x x], [y y]);
   
    % Add delete callback, which is activated when 'delete' tool selected
    set(h, 'ButtonDownFcn', {@cbdeleteobstacleline, map});
    
    % Add object to list of obstacles
    mapInfo = get(map, 'UserData');
    mapInfo.obstacleLines = [mapInfo.obstacleLines; h];
    set(map, 'UserData', mapInfo);

end

