function cbdeleteobstacleline(src, event, map)
%CBDELETEOBSTACLELINE Summary of this function goes here
%   Detailed explanation goes here
    
    % Find and remove the handle from the list
    mapInfo = get(map, 'UserData');
    if strcmp(mapInfo.state, 'Delete') 
        i = mapInfo.obstacleLines == src;
        mapInfo.obstacleLines(i) = [];
        % Delete the object
        delete(src);
        set(map, 'UserData', mapInfo);
    end


end

