function out = goalfunction(point, goal, weight)
    
    % FIXME: How to integrate multiple 'goals'?
    out = weight * (point - goal')' * (point - goal');

end
	

