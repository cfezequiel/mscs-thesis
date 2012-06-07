function cbdeletewaypoint(src, event, map, listbox)
%CBDELETEWAYPOINT Summary of this function goes here
%   Detailed explanation goes here

    mapInfo = get(map, 'UserData');
    if strcmp(mapInfo.state, 'Delete')
        % Remove waypoint from list
        i = find(mapInfo.waypoints == src);
        mapInfo.waypoints(i) = [];
        name = get(src, 'DisplayName');
        
        % Remove the waypoint from listbox
        list = get(listbox, 'String');
        for i = 1:size(list, 1)
            if strcmp(list{i}, name)
                break
            end
        end
        list(i) = [];
        set(listbox, 'String', list);

        % Delete the object
        delete(src);
        set(map, 'UserData', mapInfo);
    end
    disp('kaboodle');
    disp(mapInfo.waypoints);
end

