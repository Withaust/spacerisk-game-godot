extends Area3D

func _ready():
	if(!is_multiplayer_authority()):
		queue_free()

func _on_body_entered(_body):
	print("Player entered trigger!")


func _on_body_exited(_body):
	print("Player exited trigger!")
