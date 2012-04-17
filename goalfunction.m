function out = goalfunction(point, goal, weight)
    
    out = weight * (point - goal)' * (point - goal);

end
	

