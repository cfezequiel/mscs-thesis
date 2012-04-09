function cbaddstartpoint(map, eventdata, handles)
%CBADDSTARTPOINT Summary of this function goes here
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
    
    % Change color
    setColor(h, 'g');
    
    % Add delete callback, which is activated when 'delete' tool selected
    set(h, 'ButtonDownFcn', {@cbdeletestartpoint, map, handles.tbppSetStartPoint});  
  
    % Update map data
    mapInfo = get(map, 'UserData');
    mapInfo.startpoint = h;
    set(map, 'UserData', mapInfo);
    
    % Only one start point can be added
    set(map, 'ButtonDownFcn', []);
    
    % Disable start point tool
    set(handles.tbppSetStartPoint, 'Enable', 'off');

end

