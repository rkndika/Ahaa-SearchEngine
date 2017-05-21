<?php

  if(isset($_POST['tipe']) && isset($_POST['q']) && isset($_POST['jumlah']))
  {
    $tipe = $_POST['tipe'];
    $query = $_POST['q'];
    $jumlah = $_POST['jumlah'];
    $i=0;
    $totalqterm=0;
    $infoQuery = array();

    $data = getData($tipe, $query, $jumlah);
    $jumlahData = count($data);

    if($data[0] == 'query-start')
    {
      $i++;
      $totalqterm = (int)$data[$i];
      $i++;
      while($data[$i] != 'query-stop')
      {
        array_push($infoQuery, $data[$i]);
        $i++;
      }
      $i++;
    }

    if($i<$jumlahData && $data[$i]!="informasi" && $data[$i]!="stopword")
    {
      echo '<div class="search-panel column is-8">';
    }
    else {
      echo '
      <div class="search-panel column is-4 is-offset-4 has-text-centered">
        <div class="card is-fullwidth list-hasil notification is-primary">
          <div class="card-content">
            <div class="content">';
              switch($data[$i]){
                case "stopword" :
                  echo '
                  <i class="icon icon-basic-info is-large"></i>
                  <p class="title">Stopword</p>
                  <p class="subtitle is-6">seluruh query adalah stopword</p>';
                  echo '<script type="text/javascript">',
                       '$("#analysis-mode").addClass("is-disabled");',
                       '</script>'
                  ;
                  break;
                default :
                  echo '
                  <i class="icon icon-basic-archive is-large"></i>
                  <p class="title">Not Found</p>
                  <p class="subtitle is-6">tidak ada dokumen yang relevan terhadap query</p>';
                  break;
              }

              echo '
            </div>
          </div>
        </div>
        ';
    }

    for(; $i<$jumlahData && $data[$i]!="informasi" && $data[$i]!="stopword" ; $i++)
    {
      $infoFile = (explode("\t", $data[$i]));
      $fileName = $infoFile[0];
      $filenya = fopen("data/".$fileName, "r") or die("Unable to open file!");
      $isiFile = fread($filenya,filesize("data/".$fileName));

      // mengambil judul dan konten tulisan
      $temp = (explode("<title>", $isiFile));
      $isiFile = $temp[1];
      $temp = (explode("</title>", $isiFile));
      $judul = $temp[0];
      $isiFile = $temp[1];
      $temp = (explode("<content>", $isiFile));
      $isiFile = $temp[1];
      $temp = (explode("</content>", $isiFile));
      $konten = $temp[0];

      fclose($filenya);

      $judul = strip_tags($judul);
      $konten = substr(strip_tags($konten),0,250).'...';

      echo '
      <div class="card is-fullwidth list-hasil">
        <div class="card-content">
          <div class="content">
            <p>
              <a href="data/'.$fileName.'" class="title is-5">
                '.$judul.'
              </a>
              <br><small style="color:#00d1b2">http://cs.unsyiah.ac.id/~mndika/AHAA/data/'.$fileName.'</small>
              <br>
              <p style="text-align:justify">'.$konten.'</p>
            </p>
            <span class="tag is-primary"> Rank : '.$infoFile[1].'</span>
          </div>
        </div>
      </div>
      ';
    }
    echo '</div>';

    if(isset($data[$i]) && $data[$i]=="informasi")
    {
      $i++;
      $summary = (explode("\t", $data[$i]));
      echo '
      <div class="info-panel column is-12 hidden-item">
        <div class="columns">
          <div class="column is-4 is-offset-2">
            <div class="card has-text-centered is-fullwidth">
              <div class="card-content notification is-primary">
                <div class="content">
                  <h1 class="title is-1">'.$summary[0].'</h1>
                  Struktur Data
                </div>
              </div>
            </div>
          </div>
          <div class="column is-4">
            <div class="card has-text-centered is-fullwidth">
              <div class="card-content notification is-primary">
                <div class="content">
                  <h1 class="title is-1">'.$summary[5].' <small>seconds</small></h1>
                  Waktu Eksekusi
                </div>
              </div>
            </div>
          </div>
        </div>

        <div class="columns">

          <div class="column is-3">
            <div class="card has-text-centered is-fullwidth">
              <div class="card-content notification is-primary">
                <div class="content">
                  <h1 class="title is-1">'.$summary[1].'</h1>
                  Kata Stopword
                </div>
              </div>
            </div>
          </div>

          <div class="column is-3">
            <div class="card has-text-centered is-fullwidth">
              <div class="card-content notification is-primary">
                <div class="content">
                  <h1 class="title is-1">'.$summary[2].'</h1>
                  File Dokumen
                </div>
              </div>
            </div>
          </div>

          <div class="column is-3">
            <div class="card has-text-centered is-fullwidth">
              <div class="card-content notification is-primary">
                <div class="content">
                  <h1 class="title is-1">'.$summary[3].'</h1>
                  Kata Diindeks
                </div>
              </div>
            </div>
          </div>

          <div class="column is-3">
            <div class="card has-text-centered is-fullwidth">
              <div class="card-content notification is-primary">
                <div class="content">
                  <h1 class="title is-1">'.$summary[4].'</h1>
                  Hasil Ditampilkan
                </div>
              </div>
            </div>
          </div>

        </div>

        <div class="columns">
          <div class="column is-12">
            <div class="card has-text-centered is-fullwidth">
              <header class="card-header">
                <p class="card-header-title is-1">
                  Query
                </p>
              </header>
              <div class="card-content notification is-primary">
                <div class="content">
                  <div class="columns">';

                  for($j=0;$j<$totalqterm;$j++)
                  {
                    $dataQuery = (explode("\t", $infoQuery[$j]));

                    if($j%6 == 0)
                    {
                      echo '
                      </div>
                      <div class="columns">';
                    }

                    echo '
                    <div class="column is-2">
                      <div class="card has-text-centered is-fullwidth">
                        <div class="card-content">
                          <div class="content">
                            <h1 class="title is-3">'.$dataQuery[1].' <small>file</small></h1>
                            <small>berkaitan dengan <br> <strong>'.$dataQuery[0].'</strong></small>
                          </div>
                        </div>
                      </div>
                    </div>';

                  }
                  echo '
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      ';
    }
  }


  function getData($tipe, $query, $jumlah)
  {
    $query = strtolower($query);
    if($tipe=="Hash")
    {
      chdir("backend");
      exec("gcc -Wall -pedantic -g -o indexdb index-db.c index-tools.c util.c");
      exec("gcc -Wall -pedantic -g -o queryHash query-with-doclen-Hash.c query-tools-Hash.c util.c -lm");
      exec("./indexdb");
      exec("./queryHash '$query' $jumlah", $data);
      chdir("../");
      return $data;
  	}
    else if($tipe=="BST")
    {
      chdir("backend");
      exec("gcc -Wall -pedantic -g -o indexdb index-db.c index-tools.c util.c");
      exec("gcc -Wall -pedantic -g -o queryBST query-with-doclen-BST.c query-tools-BST.c util.c -lm");
      exec("./indexdb");
      exec("./queryBST '$query' $jumlah", $data);
      chdir("../");
      return $data;
  	}
  }
?>
