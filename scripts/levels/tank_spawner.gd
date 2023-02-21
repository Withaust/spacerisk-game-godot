extends MultiplayerSpawner

func _ready():
	if(!is_multiplayer_authority()):
		return
	var index = randi_range(0, get_spawnable_scene_count() - 1)
	add_child(load(get_spawnable_scene(index)).instantiate())
