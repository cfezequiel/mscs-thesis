function addboundaries(map)
%ADDBOUNDARIES Summary of this function goes here
%   Detailed explanation goes here

    hList(1) = line([10 10 50 50 10], [10 50 50 10 10]);
    hList(2) = line([10 10 50 50 10], [60 100 100 60 60]);
    hList(3) = line([60 60 110], [0 100 100]);
    
    mapInfo = get(map, 'UserData');
    mapInfo.boundaries = hList;
    set(map, 'UserData', mapInfo);
    
end

