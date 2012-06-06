function cbaddobstaclerect(map, eventdata, handles)
%CBADDOBSTACLELINE Add a draggable, resizable line obstacle
%   Detailed explanation goes here

    pos = get(map, 'CurrentPoint');
    x = pos(1,1); 
    y = pos(1,2);
    h = imrect(map, [x y 10 10]);
   
    % Add delete callback, which is activated when 'delete' tool selected
    set(h, 'ButtonDownFcn', {@cbdeleteobstaclerect, map});
    
    % Add object to list of obstacles
    mapInfo = get(map, 'UserData');
    mapInfo.obstacleRects = [mapInfo.obstacleRects; h];
    set(map, 'UserData', mapInfo);

end

