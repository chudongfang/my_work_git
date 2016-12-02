<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<title>OCR文本识别系统</title>
<!-- 新 Bootstrap 核心 CSS 文件 -->
<link rel="stylesheet" href="http://cdn.bootcss.com/bootstrap/3.3.0/css/bootstrap.min.css">
<!-- 可选的Bootstrap主题文件（一般不用引入） -->
<link rel="stylesheet" href="http://cdn.bootcss.com/bootstrap/3.3.0/css/bootstrap-theme.min.css">
<!-- jQuery文件。务必在bootstrap.min.js 之前引入 -->
<script src="http://cdn.bootcss.com/jquery/1.11.1/jquery.min.js"></script>

<!-- 最新的 Bootstrap 核心 JavaScript 文件 -->
<script src="http://cdn.bootcss.com/bootstrap/3.3.0/js/bootstrap.min.js"></script>


<style type="text/css">
  .form{
    position:absolute;
    left:600px;
    top:100px 

  }
  .image{
    position:absolute;
    left:10px;
    top:60px 
  }
  .retext{
    position:absolute;
    left:450px;
    top :70px;
  }
  .btn{
    position:absolute;
    left:450px;
    top :330px;
  }

  .body{
    background-image: url("./img/background.jpg");
  }
  .text{
    text-align: center;       
  }
</style>
</head>

<body class = "body">
<?php //$file_path = './img/180.jpg'?>
<nav class="navbar navbar-inverse" role="navigation">
  <div class="container-fluid">
    <!-- Brand and toggle get grouped for better mobile display -->
    <div class="navbar-header">
      <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
        <span class="sr-only">Toggle navigation</span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
      <a class="navbar-brand" href="index.php">OCR文本识别系统</a>
    </div>

    <!-- Collect the nav links, forms, and other content for toggling -->
    <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
      <ul class="nav navbar-nav navbar-right">
        <li><a href="./introduce1.php">系统介绍</a></li>
        <li><a href="./introduce1.php">识别样例</a></li>
        <li><a href="./train.php">训练</a></li>
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown">参考资源 <span class="caret"></span></a>
          <ul class="dropdown-menu" role="menu">
            <li><a href="https://github.com/thiagoalessio/tesseract-ocr-for-php">tesseract-ocr-for-php</a></li>
            <li><a href="http://getbootstrap.com/">Bootstrap</a></li>
            <li><a href="http://www.w3school.com.cn/">W3School</a></li>
            <li class="divider"></li>
            <li><a href="https://github.com/tesseract-ocr/tesseract">tesseract-ocr</a></li>
          </ul>
        </li>
      </ul>
    </div><!-- /.navbar-collapse -->
  </div><!-- /.container-fluid -->
</nav>




<?php
    $file_path =  $_GET['file_name'];
    $lan_type  =  $_GET['lan_type'];
    require '../vendor/autoload.php';
    $ocr = new \TesseractOCR($file_path);
    $string = $ocr ->lang($lan_type) ->run();

?>


<div class = "image">
   <p>
    <img src=<?php  echo $file_path;?>
         width="440" height="600" />
   </p>
</div>

<div class = "retext" style ="width :588px">
  <p>识别结果：</p>
  <textarea class="form-control" rows="25"><?php echo $string; ?></textarea>
</div>
<div class = "btn"><a href="./introduce1.php" class="btn btn-primary btn-lg active" role="button">返回</a></div>





</body>
</html>
