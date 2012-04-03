function cbremoveobstacle(map, eventdata, handles)
%CBREMOVEOBSTACLE Summary of this function goes here
%   Detailed explanation goes here
   
    mapData = get(map, 'UserData');
    mapData.state = 'Remove obstacle';
    set(map, 'UserData', mapData);  
   
end

