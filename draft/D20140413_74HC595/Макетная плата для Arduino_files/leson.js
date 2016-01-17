function Add_tovar(id_lesson){

	var id_shop=$('#add_nomer_file').attr("value");
	
		$.ajax({
        url:'/ajax/add_tovar.php'
        , type:'POST'
		, data:'id_lesson='+id_lesson+'&id_shop='+id_shop
		, success: function(res) {
			$('.add_file_tovar span').html(res);
			$('#add_nomer_file').focus();
			}
        });
	
}