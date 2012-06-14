function cbdeleteobstacle(src, eventdata)
%CBDELETEOBSTACLE Summary of this function goes here
%   Detailed explanation goes here

    % Get current position of cursor from map
    pos = get(src, 'CurrentPoint');
    xc = pos(1,1); 
    yc = pos(1,2);
    
    % Get map information
    mapInfo = get(src, 'UserData');
    
    % Get positions of all obstacles
    nObstacleRects = size(mapInfo.obstacleRects, 1);
    nObstacleCircles = size(mapInfo.obstacleCircles, 1);
    obstaclePositions = zeros(nObstacleRects + nObstacleCircles, 2);
    for i = 1:nObstacleRects
        pos = getPosition(mapInfo.obstacleRects(i));
        obstaclePositions(i, :) = pos(1, 1:2);
    end
    
    offset = nObstacleRects;
    for i = 1:nObstacleCircles
        pos = getPosition(mapInfo.obstacleCircles(i));
        obstaclePositions(offset + i, :) = pos(1, 1:2);
    end
        
    % Get distances from cursor position to each obstacle position
    D = pdist2(obstaclePositions, [xc yc]);
    [sortedD, I] = sortrows(D);
    
    % Do nothing if cursor is too far from nearest obstacle
    if sortedD(1) > 5
        return
    end
    
    % Delete the closest obstacle to the position
    iClosest = I(1, :);
    if iClosest <= offset
        h = mapInfo.obstacleRects(iClosest);
        delete(h);
        mapInfo.obstacleRects(iClosest) = [];
    else
        iClosest = iClosest - offset;
        h = mapInfo.obstacleCircles(iClosest);
        delete(h);
        mapInfo.obstacleCircles(iClosest) = [];
    end
    
    % Save map information
    set(src, 'UserData', mapInfo);
    
end

