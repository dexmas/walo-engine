enum TTileType
{
	NOTHING     = 0,
	BLOCKED     = 1,
	ROOM        = 2,
	CORRIDOR    = 3,
	PERIMETER   = 4,
	ARCH        = 5,
	LOCKED      = 6,
	TRAPPED     = 7,
	SECRET      = 8,
	PORTC       = 9,
	UNLOCKED 	= 10,
	STAIR_DN    = 11,
	STAIR_UP    = 12,
	CHEST		= 13,
	MONSTER		= 14,
	LEVER		= 15,
}

class CDungeon
{
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// directions

	di = { north = -1, south =  1, west =  0, east =  0 };
	dj = { north =  0, south =  0, west = -1, east =  1 };
	dj_dirs = null;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// stairs

	stair_end = 
	{
		north = 
		{
			walled    = [[1,-1],[0,-1],[-1,-1],[-1,0],[-1,1],[0,1],[1,1]],
			corridor  = [[0,0],[1,0],[2,0]],
			stair     = [0,0],
			next      = [1,0],
		},
		south = 
		{
			walled    = [[-1,-1],[0,-1],[1,-1],[1,0],[1,1],[0,1],[-1,1]],
			corridor  = [[0,0],[-1,0],[-2,0]],
			stair     = [0,0],
			next      = [-1,0],
		},
		west = 
		{
			walled    = [[-1,1],[-1,0],[-1,-1],[0,-1],[1,-1],[1,0],[1,1]],
			corridor  = [[0,0],[0,1],[0,2]],
			stair     = [0,0],
			next      = [0,1],
		},
		east = 
		{
			walled    = [[-1,-1],[-1,0],[-1,1],[0,1],[1,1],[1,0],[1,-1]],
			corridor  = [[0,0],[0,-1],[0,-2]],
			stair     = [0,0],
			next      = [0,-1],
		},
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// imaging

	opts = 
	{
		seed              = 0,
		n_rows            = 20,          // must be an odd number
		n_cols            = 20,          // must be an odd number
		room_min          = 3,           // minimum room size
		room_max          = 9,           // maximum room size
	};

	dungeon = null;
	tiles = {};

	// showtime
	constructor()
	{
		dj_dirs = {};
		foreach(key, item in dj)
		{
			dj_dirs.rawset(item, key);
		}
	}
	
	function Generate(szI, szJ, minRoom, maxRoom)
	{
		opts.seed = ::Math.Rand(0, 10000);
		opts.n_rows            = szJ,          // must be an odd number
		opts.n_cols            = szI,          // must be an odd number
		opts.room_min          = minRoom,           // minimum room size
		opts.room_max          = maxRoom,           // maximum room size
		dungeon = clone(opts);
		create_dungeon();
	}

	function custom_compare(a,b)
	{
		if(a < b)
		{
			return 1;
		}
		else if(a > b)
		{
			return -1;
		}
		
		return 0;
	}
	
	
	// create dungeon
	function create_dungeon() 
	{
	  dungeon.n_i <- (dungeon.n_rows / 2).tointeger();
	  dungeon.n_j <- (dungeon.n_cols / 2).tointeger();
	  dungeon.n_rows = dungeon.n_i * 2;
	  dungeon.n_cols = dungeon.n_j * 2;
	  dungeon.max_row <- dungeon.n_rows - 1;
	  dungeon.max_col <- dungeon.n_cols - 1;
	  dungeon.n_rooms <- 0;
	
	  local max = dungeon.room_max;
	  local min = dungeon.room_min;
	  dungeon.rooms_base <- ((min + 1) / 2).tointeger();
	  dungeon.rooms_radix <- ((max - min) / 2).tointeger() + 1;
	
	  init_cells();
	  
	  pack_rooms();
	  
	  open_rooms();
	  emplace_stairs();
	  clean_dungeon();
	  
	  place_chest();
	}
	
	function place_chest()
	{
		foreach(room in dungeon.rooms)
		{
			local max_monsters = (room.area/3000).tointeger() + 1;
			local max_chest = (room.area/3000).tointeger() + 1;
			
			local count_monsters = ::Math.Rand(0, max_monsters);
			local count_chest = ::Math.Rand(0, max_chest);

			while(count_monsters > 0)
			{
				local r = ::Math.Rand(room.row,room.south);
				local c = ::Math.Rand(room.col,room.east);
				
				if(dungeon.cell[r][c] == TTileType.ROOM || dungeon.cell[r][c] == TTileType.CORRIDOR)
				{
					dungeon.cell[r][c] = TTileType.MONSTER;
					count_monsters -= 1;
				}
			}
			
			while(count_chest > 0)
			{
				local r = ::Math.Rand(room.row,room.south);
				local c = ::Math.Rand(room.col,room.east);
				
				if(dungeon.cell[r][c] == TTileType.ROOM || dungeon.cell[r][c] == TTileType.CORRIDOR)
				{
					dungeon.cell[r][c] = TTileType.CHEST;
					count_chest -= 1;
				}
			}
			
			/*local countPortc = 0;
			local countLocked = 0;
			if(room.rawin("door"))
			{
				foreach(door in room.doorTypes)
				{
					if(door == TTileType.PORTC)
					{
						countPortc += 1;
					}
					else if(door == TTileType.LOCKED)
					{
						countLocked += 1;
					}
				}
			}
			
			while(countPortc > 0)
			{
				local r = ::Math.Rand(room.row,room.south);
				local c = ::Math.Rand(room.col,room.east);
				
				if(dungeon.cell[r][c] == TTileType.ROOM || dungeon.cell[r][c] == TTileType.CORRIDOR)
				{
					dungeon.cell[r][c] = TTileType.LEVER;
					countPortc -= 1;
				}
			}*/
		}
	}
	
	// initialize cells
	function init_cells()
	{
	  dungeon.cell <- [];
	  for (local r = 0; r <= dungeon.n_rows; r++)
	  {
	  	local row = [];
	    for (local c = 0; c <= dungeon.n_cols; c++)
	    {
	      row.push(TTileType.NOTHING);
	    }
	    dungeon.cell.push(row);
	  }
	
	  mask_cells();
	}
	
	// mask cells
	function mask_cells()
	{
	 local mask = [[1,1,1],[1,0,1],[1,1,1]];
	  
	  local r_x = (mask.len() * 1.0 / dungeon.n_rows);
	  local c_x = (mask[0].len() * 1.0 / dungeon.n_cols);
	
	  for (local r = 0; r <= dungeon.n_rows; r++) {
	    for (local c = 0; c <= dungeon.n_cols; c++) {
	      dungeon.cell[r][c] = true && !(mask.len() > (r * r_x) && mask[r * r_x].len());
	    }
	  }
	}
	
	function pack_rooms()
	{
	  for (local i = 0; i < dungeon.n_i; i++)
	  {
	    local r = (i * 2) + 1;
	    for (local j = 0; j < dungeon.n_j; j++)
	    {
	      local c = (j * 2) + 1;
		
		  if(dungeon.cell[r][c] && TTileType.ROOM)
		  {
		  	continue;
		  }
		  if((i == 0 || j == 0) && ::Math.Rand(0,2) > 1)
	      {
		  	continue;
		  }
	
	      emplace_room({ i = i, j = j });
	    }
	  }
	}
	
	
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// emplace room
	
	function emplace_room(_proto) {
	  local proto = _proto;
	
	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	  // room position and size
	
	  proto = set_room(proto);
	
	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	  // room boundaries
	
	  local r1 = ( proto.i * 2) + 1;
	  local c1 = ( proto.j * 2) + 1;
	  local r2 = ((proto.i + proto.height) * 2) - 1;
	  local c2 = ((proto.j + proto.width ) * 2) - 1;
	
	  if (r1 < 1 || r2 > dungeon.max_row)
	  {
	  	return;
	  }
	  
	  if (c1 < 1 || c2 > dungeon.max_col)
	  {
	  	return;
	  }
	
	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	  // check for collisions with existing rooms
	
	  local hit = sound_room(r1,c1,r2,c2);
	  if(hit == null)
	  {
	  	return;
	  }
	  
	  local room_id = null;
	
	  if (hit.len() == 0)
	  {
	    room_id = dungeon.n_rooms + 1;
	    dungeon.n_rooms = room_id;
	  }
	  else
	  {
	    return;
	  }
	  dungeon.rawset("last_room_id", room_id);
	
	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	  // emplace room
	
	  for (local r = r1; r <= r2; r++)
	  {
	    for (local c = c1; c <= c2; c++)
	    {
	      dungeon.cell[r][c] = TTileType.ROOM;
	    }
	  }
	  local height = ((r2 - r1) + 1) * 10;
	  local width = ((c2 - c1) + 1) * 10;
	
	  local room_data = {
	    id = room_id, row = r1, col = c1,
	    north = r1, south = r2, west = c1, east = c2,
	    height = height, width = width, area = (height * width)
	  };

	  if(!dungeon.rawin("rooms"))
	  {
	  	dungeon.rooms <- {};
	  }

	  dungeon.rooms.rawset(room_id, room_data);
	
	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	  // block corridors from room boundary
	  // check for door openings from adjacent rooms
	
	  for (local r = r1 - 1; r <= r2 + 1; r++)
	  {
	    if(!(dungeon.cell[r][c1 - 1] == TTileType.ROOM || dungeon.cell[r][c1 - 1] == TTileType.CORRIDOR))
	    {
	      dungeon.cell[r][c1 - 1] = TTileType.PERIMETER;
	    }
	    if(!(dungeon.cell[r][c2 + 1] == TTileType.ROOM || dungeon.cell[r][c2 + 1] == TTileType.CORRIDOR))
	    {
	      dungeon.cell[r][c2 + 1] = TTileType.PERIMETER;
	    }
	  }
	  for (local c = c1 - 1; c <= c2 + 1; c++)
	  {
	    if (!(dungeon.cell[r1 - 1][c] == TTileType.ROOM || dungeon.cell[r1 - 1][c] == TTileType.CORRIDOR))
	    {
	      dungeon.cell[r1 - 1][c] = TTileType.PERIMETER;
	    }
	    if (!(dungeon.cell[r2 + 1][c] == TTileType.ROOM || dungeon.cell[r2 + 1][c] == TTileType.CORRIDOR))
	    {
	      dungeon.cell[r2 + 1][c] = TTileType.PERIMETER;
	    }
	  }
	
	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// room position and size
	
	function set_room(proto)
	{
	  proto.rawset("height", ::Math.Rand(1,dungeon.rooms_radix) + dungeon.rooms_base);
	  proto.rawset("width", (::Math.Rand(1,dungeon.rooms_radix)).tointeger() + dungeon.rooms_base);
	  
	  return proto;
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// sound room
	
	function sound_room(r1,c1,r2,c2)
	{
	  local hit = {};
	
	  for (local r = r1; r <= r2; r++)
	  {
	    for (local c = c1; c <= c2; c++)
	    {
	      if (dungeon.cell[r][c] == TTileType.BLOCKED)
	      {
	        return null;
	      }
	      if (dungeon.cell[r][c] == TTileType.ROOM)
	      {
	        local id = (dungeon.cell[r][c]);
	        if(!hit.rawin(id))
	        {
	        	hit.rawset(id, 1);
	        }
	        else
	        {
	        	hit[id] += 1;
	        }
	      }
	    }
	  }
	  
	  return hit;
	}
	
	
	// emplace openings for doors and corridors
	
	function open_rooms()
	{
		dungeon.rawset("connect", {});
		for (local id = 1; id <= dungeon.n_rooms; id++)
		{
			dungeon.rawset("connect", {});
			open_room(dungeon.rooms[id]);
		}
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// emplace openings for doors and corridors
	
	function open_room(room)
	{
	  local list = door_sills(room);
	  if(!list)
	  {
	    return;
	  }
	  local n_opens = alloc_opens(room);

	  for (local i = 0; i < n_opens; i++)
	  {
	  	local cancel = false;
	  	local list_id = ::Math.Rand(0, (list.len()-1));
	  	if(list.len() == 0)
	  	{
	  		break;
	  	}
	    local sill = list[list_id];
	    list.remove(list_id);
	    if(!sill)
	    {
	    	break;
	    }
	    local door_r = sill.door_r;
	    local door_c = sill.door_c;
	    local door_cell = dungeon.cell[door_r][door_c];

	    if(door_cell != TTileType.PERIMETER)
	    {
	    	continue;
	    }

		local out_id = sill.out_id;
	    //if (out_id)
	    //{
	      //local connect = room.id+","+out_id;
	      local connect = room.id+","+sill.dir;
	      if(!dungeon.connect.rawin(connect))
	      {
	      	dungeon.connect.rawset(connect, 1);
	      }
	      else
	      {
	      	continue;
	      }
	    //}
	    local open_r = sill.sill_r;
	    local open_c = sill.sill_c;
	    local open_dir = sill.dir;
	
	    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	    // open door
	
	    for (local x = 0; x < 3; x++)
	    {
	      local r = open_r + (di[open_dir] * x);
	      local c = open_c + (dj[open_dir] * x);
		
		  if(dungeon.cell[r][c].tointeger() == TTileType.NOTHING)
		  {
		  	cancel = true;
		  	continue;
		  }
	      dungeon.cell[r][c] = TTileType.CORRIDOR;
	    }
	    if(cancel)
	    {
	    	dungeon.cell[door_r][door_c] = TTileType.PERIMETER;
	    	continue;
	    }
	    
	    local door_type = door_type();
	    local door = { row = door_r, col = door_c };
		dungeon.cell[door_r][door_c] = door_type;

	    if (out_id)
	    {
	    	door.out_id <- out_id;
	    }
	    if (door)
	    {
	    	if(!room.rawin("door"))
	    	{
	    		room.rawset("door", {});
	    	}
	    	
	    	if(!room.rawin("doorTypes"))
	    	{
	    		room.rawset("doorTypes", []);
	    	}
	    	
	    	if(!room.door.rawin(open_dir))
	    	{
	    		room.door.rawset(open_dir, []);
	    	}
	    	
	    	room.door[open_dir].push(door);
	    	room.doorTypes.push(door_type);
	    }
	  }
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// allocate number of opens
	
	function alloc_opens(room)
	{
	  local room_h = ((room.south - room.north) / 2) + 1;
	  local room_w = ((room.east - room.west) / 2) + 1;
	  local flumph = (::Math.Sqrt(room_w * room_h)).tointeger();
	  local n_opens = flumph + ::Math.Rand(0, flumph);

	  return n_opens;
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// list available sills
	
	function door_sills(room)
	{
	  local list = [];
	  local result = null;
	
	  if (room.north >= 3)
	  {
	    for (local c = room.west; c <= room.east; c += 2)
	    {
	      local proto = check_sill(room,room.north,c,"north");
	      if (proto)
	      {
	      	list.push(proto);
	      }
	    }
	  }
	  if (room.south <= (dungeon.n_rows - 3))
	  {
	    for (local c = room.west; c <= room.east; c += 2)
	    {
	      local proto = check_sill(room,room.south,c,"south");
	      if (proto)
	      {
	      	list.push(proto);
	      }
	    }
	  }
	  if (room.west >= 3) {
	    for (local r = room.north; r <= room.south; r += 2)
	    {
	      local proto = check_sill(room,r,room.west,"west");
	      if (proto)
	      {
	      	list.push(proto);
	      }
	    }
	  }
	  if (room.east <= (dungeon.n_cols - 3))
	  {
	    for (local r = room.north; r <= room.south; r += 2)
	    {
	      local proto = check_sill(room,r,room.east,"east");
	      if (proto)
	      {
	      	list.push(proto);
	      }
	    }
	  }
	  
	  if(list.len() > 0)
	  {
	  	result = list[::Math.Rand(0, list.len()-1)];
	  }
	  
	  return list;
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// check sill
	
	function check_sill(room,sill_r,sill_c,dir)
	{
	  local door_r = sill_r + di[dir];
	  local door_c = sill_c + dj[dir];
	  local door_cell = dungeon.cell[door_r][door_c];

	  	 if(door_cell != TTileType.PERIMETER)
	  	 {
	     	return;
	  	 }
	     
	  local out_r  = door_r + di[dir];
	  local out_c  = door_c + dj[dir];
	  local out_cell = dungeon.cell[out_r][out_c];
	  
	  if (out_cell == TTileType.BLOCKED)
	  {
	     return;
	  }
	
	  local out_id = null; 
	  if (out_cell == TTileType.ROOM)
	  {
	    out_id = out_cell;
	    if (out_id == room.id)
	    {
	    	return;
	    }
	  }

	  return {
	    sill_r    = sill_r,
	    sill_c    = sill_c,
	    dir       = dir,
	    door_r    = door_r,
	    door_c    = door_c,
	    out_id    = out_id,
	  };
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// random door type
	
	function door_type() 
	{
	  local i = ::Math.Rand(0, 110);
	
	  if(i < 15)
	  {
	    return TTileType.ARCH;
	  }
	  else 
	  if(i < 60)
	  {
	    return TTileType.UNLOCKED;
	  } 
	  else 
	  if (i < 75)
	  {
	    return TTileType.LOCKED;
	  }
	  else 
	  if(i < 90)
	  {
	    return TTileType.TRAPPED;
	  } 
	  else 
	  if(i < 100)
	  {
	    return TTileType.SECRET;
	  }
	  else
	  {
	    return TTileType.PORTC;
	  }
	}
	
	// emplace stairs
	
	function emplace_stairs()
	{
	  local n = 2;
	  local countStairs = 0; 
	  if(n <= 0)
	  {
	  	return;
	  }
	     
	  local list = stair_ends();
	     
	  if(!list)
	  {
	  	return;
	  }
	
	  while(countStairs != n)
	  //for (local i = 0; i < n; i++)
	  {
	    local stair = list[::Math.Rand(0,list.len()-1)];
	    if(!stair)
	    {
	    	continue;
	    }
	    
	    local r = stair.row;
	    local c = stair.col;
	    //local type = ::Math.Rand(0,2);
	
		if(dungeon.cell[r][c].tointeger() == TTileType.NOTHING)
	    {
	    	continue;
	    }
	    
	    if(!dungeon.rawin("stairs"))
	    {
	    	dungeon.rawset("stairs", []);
	    	dungeon.cell[r][c] = TTileType.STAIR_UP;
	    	stair.key <- "up";
	    }
	    else 
	    {
	      dungeon.cell[r][c] = TTileType.STAIR_DN;
	      stair.key <- "down";
	    }
	    
	    dungeon.stairs.push(stair);
	    
	    countStairs++;
	  }
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// list available ends
	
	function stair_ends()
	{
	  local list = [];
	  for (local i = 0; i < dungeon.n_i; i++)
	  {
	    local r = (i * 2) + 1;
	    for (local j = 0; j < dungeon.n_j; j++)
	    {
	      local c = (j * 2) + 1;
			if(dungeon.cell[r][c] == TTileType.CORRIDOR)
			{
				continue;
			}
	      	if(dungeon.cell[r][c] == null)
	      	{
	      		continue;
	      	}
	      foreach(dir, item in stair_end)
	      {
	        //if (check_tunnel(r,c,stair_end[dir]))
	        if (true)
	        {
		        local end = { row = r, col = c };
		        local n = stair_end[dir].next;
		        end.next_row <- end.row + n[0];
		        end.next_col <- end.col + n[1];
		
		        list.push(end);
		        continue;
	        }
	      }
	    }
	  }

	  return list;
	}
	
	
	// final clean-up
	
	function clean_dungeon()
	{
	  fix_doors();
	  empty_blocks();
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// check tunnel
	
	function check_tunnel(r,c,check)
	{
	  local list = null;
	
	  if (list = check.corridor)
	  {
		  foreach(p in list)
		  {
		  	if(!dungeon.cell[r+p[0]][c+p[1]] == TTileType.CORRIDOR)
		  	{
		  		return 0;	
		  	}
		  }
	  }
	  if (list = check.walled)
	  {
		  foreach(p in list)
		  {
		  	if (dungeon.cell[r+p[0]][c+p[1]] == null)
		  	{
		  		return 0;	
		  	}
		  }
	  }
	  return 1;
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// fix door lists
	
	function fix_doors()
	{
	  local fixed = null;
	
	  foreach(room in dungeon.rooms)
	  {
	  	if(!room.rawin("door"))
	  	{
	  		continue;
	  	}
	    foreach(dir, item in room.door)
	    {
	      local shiny = null;
	      foreach(door in room.door[dir])
	      {
	        local door_r = door.row;
	        local door_c = door.col;
	        local door_cell = dungeon.cell[door_r][door_c];
	           if(!(door_cell == null))
	           {
	           		continue;
	           }
	
	        if (fixed[door_r][door_c])
	        {
	          shiny.push(door);
	        }
	        else
	        {
	          local out_id = null;
	          if (out_id = door.out_id)
	          {
		          out_dir = opposite[dir];
		          dungeon.rooms[out_id].door[out_dir].push(door);
	          }
	          shiny.push(door);
	          fixed[door_r][door_c] = 1;
	        }
	      }
	      if (shiny)
	      {
	        room.door[dir] = shiny;
	        dungeon.door.push(shiny);
	      }
	      else
	      {
	        delete room.door[dir];
	      }
	    }
	  }
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// empty blocks
	
	function empty_blocks()
	{
	  for (local r = 0; r <= dungeon.n_rows; r++)
	  {
	    for (local c = 0; c <= dungeon.n_cols; c++)
	    {
	    	if (dungeon.cell[r][c] == TTileType.BLOCKED)
	    	{
	    		dungeon.cell[r][c] = TTileType.NOTHING;	
	    	}
	    }
	  }
	}
	
	function destroy()
	{
		foreach(i, row in dungeon.cell)
		{
			foreach(j, cell in dungeon.cell)
			{
				dungeon.cell[i][j] = null;
			}
		}
		
		foreach(key, room in dungeon.rooms)
		{
			dungeon.rooms[key] = null;
		}
		
		foreach(key, stair in dungeon.stairs)
		{
			dungeon.stairs[key] = null;
		}
		
		foreach(key, item in dungeon)
		{
			dungeon[key] = null;
			dungeon.rawdelete(key);
		}
		
		dungeon = null;
	}
	
}