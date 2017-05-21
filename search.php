<?php
  $tipe = $_GET['tipe'];
  $query = $_GET['q'];
  $jumlah = $_GET['jumlah'];
?>

<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>Ahaa!</title>
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <link rel="icon" type="image/png" href="favicon.png">
  <link rel="stylesheet" href="assets/css/bulma.css" media="screen">
  <link rel="stylesheet" href="assets/css/linea.css" media="screen">
  <link rel="stylesheet" href="assets/css/style.css" media="screen">
</head>
<body>
  <section class="hero is-primary">
    <!-- Hero header: will stick at the top -->
    <div class="hero-head">
      <header class="nav">
        <div class="container">
          <div class="nav-left">
          </div>
          <span id="nav-toggle" class="nav-toggle">
            <span></span>
            <span></span>
            <span></span>
          </span>
          <div id="nav-menu" class="nav-right nav-menu">
            <a href="http://cs.unsyiah.ac.id/~mndika/#about" class="nav-item">
              <i class="icon icon-basic-heart is-medium"></i> About
            </a>
            <a href="http://cs.unsyiah.ac.id/~mndika/#project" class="nav-item">
              <i class="icon icon-basic-flag1 is-medium"></i> My Project
            </a>
            <a href="http://cs.unsyiah.ac.id/~mndika/#contact" class="nav-item">
              <i class="icon icon-basic-smartphone is-medium"></i> Contact
            </a>
            <span class="nav-item">
              <a href="index.php" class="button is-primary is-inverted">
                <span class="icon">
                  <i class="icon icon-basic-home"></i>
                </span>
                <span></span>
              </a>
            </span>
          </div>
        </div>
      </header>
    </div>

    <!-- Hero content: will be in the middle -->
    <div class="hero-body">

      <div class="container is-hidden-mobile">
        <div class="level">
          <div class="level-left">
            <div class="level-item">
              <img src="assets/images/logo.png" style="width: 200px;">
            </div>
            <div class="level-item">

              <p class="control has-addons">
                <span class="select is-medium">
                  <select id="" name="tipe" class="tipe select-search" >
                    <option value="Hash" <?php if($tipe == "Hash") echo "selected" ?>>Hash</option>
                    <option value="BST" <?php if($tipe == "BST") echo "selected" ?>>BST</option>
                  </select>
                </span>
                <input id="" name="q" class="q input is-medium input-search" type="text" placeholder="Search" value="<?php echo $query ?>">
                <span class="select is-medium">
                  <select id="" name="jumlah" class="jumlah select-search">
                    <option value="10" <?php if($jumlah == 10) echo "selected" ?>>10</option>
                    <option value="20" <?php if($jumlah == 20) echo "selected" ?>>20</option>
                    <option value="30" <?php if($jumlah == 30) echo "selected" ?>>30</option>
                    <option value="40" <?php if($jumlah == 40) echo "selected" ?>>40</option>
                    <option value="50" <?php if($jumlah == 50) echo "selected" ?>>50</option>
                  </select>
                </span>
                <button class="search button is-danger is-medium has-text-centered">
                  <i class="icon icon-basic-magnifier title is-3"></i>
                </button>
              </p>

            </div>
          </div>
        </div>
      </div>

      <div class="container has-text-centered is-hidden-tablet">
        <img src="assets/images/logo.png" style="width: 200px;">

        <p class="control has-addons has-addons-centered">
          <span class="select">
            <select id="" name="tipe" class="tipe-m select-search" >
              <option value="Hash" <?php if($tipe == "Hash") echo "selected" ?>>Hash</option>
              <option value="BST" <?php if($tipe == "BST") echo "selected" ?>>BST</option>
            </select>
          </span>
          <input id="" name="q" class="q-m input input-search" type="text" placeholder="Search" style="width: 150px" value="<?php echo $query ?>">
          <span class="select">
            <select id="" name="jumlah" class="jumlah-m select-search">
              <option value="10" <?php if($jumlah == 10) echo "selected" ?>>10</option>
              <option value="20" <?php if($jumlah == 20) echo "selected" ?>>20</option>
              <option value="30" <?php if($jumlah == 30) echo "selected" ?>>30</option>
              <option value="40" <?php if($jumlah == 40) echo "selected" ?>>40</option>
              <option value="50" <?php if($jumlah == 50) echo "selected" ?>>50</option>
            </select>
          </span>
          <button class="search-m button is-danger has-text-centered">
            <i class="icon icon-basic-magnifier subtitle is-4"></i>
          </button>
        </p>

      </div>
    </div>
  </section>
  <nav class="nav has-shadow navmenu">
    <div class="container">
      <div class="nav-left">
        <a id="search-mode" class="nav-item is-tab is-active">Search</a>
        <a id="analysis-mode" class="nav-item is-tab">Analysis</a>
      </div>
    </div>
  </nav>



  <section class="section">
    <div class="container">
      <div id="loading" class="columns hidden-item">
        <div class="column is-12 has-text-centered">
          <a class="button is-primary is-loading is-large" style="margin: 50px 0;">Load</a>
        </div>
      </div>

      <div id="hasil" class="columns hidden-item"></div>

    </div>
  </section>
  <nav class="nav has-shadow navmenu"></nav>
  <section class="hero is-primary">
    <div class="hero-body">
      <div class="container">
        <h1 class="title">
          Ahaa! <small>YES, IT IS !</small>
        </h1>
        <h2 class="subtitle">
          copyright &copy; 2017 by <a href="http://cs.unsyiah.ac.id/~mndika"><strong>MRA</strong></a>
        </h2>
      </div>
    </div>
  </section>

  <script src="assets/js/jquery-2.2.0.min.js" charset="utf-8"></script>
  <script src="assets/js/app.js" charset="utf-8"></script>
  <script src="assets/js/bulma.js" charset="utf-8"></script>
</body>
</html>
