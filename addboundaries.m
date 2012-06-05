function addboundaries(map)
%ADDBOUNDARIES Summary of this function goes here
%   Detailed explanation goes here

    % Construct lines
    b = zeros(10, 4);
    b(1, :) = line([10 10], [10 50]);
    b(2, :) = line([10 50], [50 50]);
    b(3, :) = line([50 50], [50 10]);    
    b(4, :) = line([50 10], [10 10]);    
    b(5, :) = line([10 10], [60 100]);
    b(6, :) = line([10 50], [100 100]);
    b(7, :) = line([50 50], [100 60]);
    b(8, :) = line([50 10], [60 60]);
    b(9, :) = line([60 60], [0 100]);
    b(10, :) = line([60 110], [100 100]);
        
    % Store boundary data
    mapInfo = get(map, 'UserData');
    mapInfo.boundaries = b;
    set(map, 'UserData', mapInfo);
    
end

