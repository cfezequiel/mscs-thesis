function cbdeleteobstaclecircle(src, event, map)
%CBDELETEOBSTACLECIRCLE Summary of this function goes here
%   Detailed explanation goes here
   
    disp('kaboodle');
    
    % Find and remove the handle from the list
    mapInfo = get(map, 'UserData');
    if strcmp(mapInfo.state, 'Delete') 
        i = mapInfo.obstacleCircles == src;
        mapInfo.obstacleCircles(i) = [];
        % Delete the object
        delete(src);
        set(map, 'UserData', mapInfo);
    end

end

