function cbdeletestartpoint(src, event, map, setStartPointTool)
%CBDELETESTARTPOINT Summary of this function goes here
%   Detailed explanation goes here

    mapInfo = get(map, 'UserData');
    if strcmp(mapInfo.state, 'Delete')
        % Remove waypoint from list
        mapInfo.startPoint = [];
        
        % Delete the object
        delete(src);
        set(map, 'UserData', mapInfo);
        
        % Re-enable start point tool
        set(setStartPointTool, 'Enable', 'on');
    end

end

