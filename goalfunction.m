function out = goalfunction(X, Y, goal, weight)
    
    % FIXME: How to integrate multiple 'goals'?
    
    nX = size(X, 2);
    nY = size(Y, 2);

    v = zeros(nY, nX);
    for i=1:nX
        for j=1:nY
            v(j, i) = weight * ([X(i); Y(j)] - goal')' * ...
                               ([X(i); Y(j)] - goal');
        end
    end
	out = v;
	

