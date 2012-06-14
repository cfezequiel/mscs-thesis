function cbaddobstaclecircle(map, eventdata, handles)
%CBADDOBSTACLE Summary of this function goes here
%   Detailed explanation goes here

    pos = get(map, 'CurrentPoint');
    x = pos(1,1); 
    y = pos(1,2);
    h = imellipse(map, [x y 10 10]);
    setFixedAspectRatioMode(h, 1);
        
    % Add delete callback, which is activated when 'delete' tool selected
    % FIXME: bug, cannot seem to click the obstacle
    %set(h, 'ButtonDownFcn', {@cbdeleteobstaclecircle, map});  
    
    % Add object to list of obstacles
    mapInfo = get(map, 'UserData');
    mapInfo.obstacleCircles = [mapInfo.obstacleCircles; h];
    set(map, 'UserData', mapInfo);

end

