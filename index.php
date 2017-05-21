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
  <section class="hero is-primary is-fullheight">
    <div id="overlay"></div>
    <!-- Hero header: will stick at the top -->
    <div class="hero-head">
      <header class="nav">
        <div class="container">
          <div class="nav-left">
            <a href="index.php" class="nav-item is-hidden-mobile">
              <h1 class="title is-3">
                Ahaa!
              </h1>
            </a>
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
      <div class="container has-text-centered">

        <img src="assets/images/logo.png" style="min-width: 300px;width: 50%; margin-bottom: 5%; margin-top:-5%;">

        <form action="search.php" method="get">
          <p class="control has-addons has-addons-centered is-hidden-mobile">
            <span class="select is-large">
              <select name="tipe" class="select-search" >
                <option value="Hash">Hash</option>
                <option value="BST">BST</option>
              </select>
            </span>
            <input name="q" class="input is-large input-search" type="text" placeholder="Search">
            <span class="select is-large">
              <select name="jumlah" class="select-search">
                <option value="10">10</option>
                <option value="20">20</option>
                <option value="30">30</option>
                <option value="40">40</option>
                <option value="50">50</option>
              </select>
            </span>
            <button class="button is-danger is-large has-text-centered">
              <i class="icon icon-basic-magnifier title is-3"></i>
            </button>
          </p>
        </form>

        <form action="search.php" method="get">
          <p class="control has-addons has-addons-centered is-hidden-tablet">
            <span class="select">
              <select name="tipe" class="select-search" >
                <option value="Hash">Hash</option>
                <option value="BST">BST</option>
              </select>
            </span>
            <input name="q" class="input input-search" type="text" placeholder="Search" style="width: 150px">
            <span class="select">
              <select name="jumlah" class="select-search">
                <option value="10">10</option>
                <option value="20">20</option>
                <option value="30">30</option>
              </select>
            </span>
            <button class="button is-danger has-text-centered">
              <i class="icon icon-basic-magnifier subtitle is-4"></i>
            </button>
          </p>
        </form>

      </div>
    </div>

    <!-- Hero footer: will stick at the bottom -->
    <div class="hero-foot has-text-centered">
      <h2 class="subtitle copyright">Copyright &copy; 2017 by MRA</h2>
    </div>

  </section>

  <script src="assets/js/jquery-2.2.0.min.js" charset="utf-8"></script>
  <script src="assets/js/bulma.js" charset="utf-8"></script>
  <script src="assets/js/app.js" charset="utf-8"></script>

</body>
</html>
