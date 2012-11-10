function [out] = setstaticboundaries(mapInfo)
%SETSTATICBOUNDARIES Set the static (fixed) boundaries of the map

    % Set the robot-wall gap value
    gap = 1;
    
    % Define the vertices of the static obstacles
    xStatObs = [7 7 61;
                7 7 61;
                54 54 149;
                54 54 149];
    yStatObs = [7 111 0;
                104 208 208;
                104 208 208;
                7 111 0];
        
    % Expand the obstacles to avoid the robot bumping into them
    xStatObsExp = xStatObs;
    xStatObsExp([1,2,5,6,9,10]) = xStatObsExp([1,2,5,6,9,10]) - gap;
    xStatObsExp([3,4,7,8,11,12]) = xStatObsExp([3,4,7,8,11,12]) + gap;
    yStatObsExp = yStatObs;
    yStatObsExp([1,4,5,8,9,12]) = yStatObsExp([1,4,5,8,9,12]) - gap;
    yStatObsExp([2,3,6,7,10,11]) = yStatObsExp([2,3,6,7,10,11]) + gap;
    
    % Plot the extended obstacles
    patch(xStatObsExp, yStatObsExp, 'k');
    
    % Plot the static obstacles
    patch(xStatObs, yStatObs, 'g');
        
    % Store boundary data
    mapInfo.staticObstacles.X = xStatObsExp;
    mapInfo.staticObstacles.Y = yStatObsExp;
    
    out = mapInfo;
    
end

