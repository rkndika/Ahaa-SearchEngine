$(document).ready(function()
{

  updateContent(getInputData());

  $('.search').click(function()
  {
    var data = getInputData()
    var href = "?tipe="+data[0]+"&q="+data[1]+"&jumlah="+data[2];
    $("#analysis-mode").removeClass("is-disabled");
    updateContent(data);
    window.history.pushState({href: href}, '', href);
  });

  $('.search-m').click(function()
  {
    var data = getInputData('mobile')
    var href = "?tipe="+data[0]+"&q="+data[1]+"&jumlah="+data[2];
    $("#analysis-mode").removeClass("is-disabled");
    updateContent(data);
    window.history.pushState({href: href}, '', href);
  });

  window.addEventListener('popstate', function(e)
  {
    if(e.state)
        location.reload();
  });

  $('.q').keyup(function(e){
    if(e.keyCode == 13){
      var data = getInputData()
      var href = "?tipe="+data[0]+"&q="+data[1]+"&jumlah="+data[2];
      $("#analysis-mode").removeClass("is-disabled");
      updateContent(data);
      window.history.pushState({href: href}, '', href);
    }
  });

  $('.q-m').keyup(function(e){
    if(e.keyCode == 13){
      var data = getInputData('mobile')
      var href = "?tipe="+data[0]+"&q="+data[1]+"&jumlah="+data[2];
      $("#analysis-mode").removeClass("is-disabled");
      updateContent(data);
      window.history.pushState({href: href}, '', href);
    }
  });

  $('#search-mode').click(function()
  {
    changeMode('search');
  });

  $('#analysis-mode').click(function()
  {
    changeMode('analysis');
  });
});

function changeMode($mode)
{
  if($mode == 'search')
  {
    $('#analysis-mode').removeClass('is-active');
    $('#search-mode').removeClass('is-active');
    $('#search-mode').addClass('is-active');
    $('.info-panel').fadeOut('fast','swing',function(){
      $('.search-panel').fadeIn('fast','swing',function(){
      });
    });
  }
  else {
    $('#search-mode').removeClass('is-active');
    $('#analysis-mode').removeClass('is-active');
    $('#analysis-mode').addClass('is-active');
    $('.search-panel').fadeOut('fast','swing',function(){
      $('.info-panel').fadeIn('fast','swing',function(){
      });
    });
  }
}

function getData($tipe, $q, $jumlah)
{
  var jqXHR = $.ajax({
    type: "POST",
    url: "backend.php",
    async: false,
    data: ({
      tipe: $tipe,
      q: $q,
      jumlah: $jumlah
    })
  });
  return jqXHR.responseText;
}

function updateContent($data)
{
  $('#analysis-mode').removeClass('is-disabled');
  changeMode('search');
  $('#hasil').fadeOut('fast','swing',function(){
    $('#loading').fadeIn('fast','swing',function(){
      $('#hasil').addClass('hidden-item');
      $('#loading').removeClass('hidden-item');

      $data = getData($data[0],$data[1],$data[2]);
      $('#hasil').empty();
      $('#hasil').append($data);

      $('#loading').fadeOut('slow','swing',function(){
        $('#hasil').fadeIn('slow','swing',function(){
          $('#loading').addClass('hidden-item');
          $('#hasil').removeClass('hidden-item');

        });
      });
    });
  });


}

function getInputData($mode)
{
  var $tipe, $q, $jumlah, $data, $href;
  if($mode == 'mobile'){
    $tipe = $('.tipe-m option:selected').val();
    $q = $('.q-m').val();
    $jumlah = $('.jumlah-m option:selected').val();
    $('.tipe').val($tipe);
    $('.q').val($q);
    $('.jumlah').val($jumlah);
  }
  else{
    $tipe = $('.tipe option:selected').val();
    $q = $('.q').val();
    $jumlah = $('.jumlah option:selected').val();
    $('.tipe-m').val($tipe);
    $('.q-m').val($q);
    $('.jumlah-m').val($jumlah);
  }
  return $data = [$tipe,$q,$jumlah];
}
