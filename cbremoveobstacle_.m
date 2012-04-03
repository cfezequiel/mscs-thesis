function cbremoveobstacle_(src, event, map)
%CBREMOVEOBSTACLE_ Summary of this function goes here
%   Detailed explanation goes here
    mapData = get(map, 'UserData');
    if strcmp(mapData.state, 'Remove obstacle') 
        i = find(mapData.obstacles == src);
        mapData.obstacles(i) = [];
        delete(src);
        set(map, 'UserData', mapData);

end

