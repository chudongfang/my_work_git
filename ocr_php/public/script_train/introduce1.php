<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,height=device-height,inital-scale=1.0,maximum-scale=1.0,user-scalable=no;">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black">
<meta name="format-detection" content="telephone=no">
<title>OCR文本识别系统</title>
<!-- 新 Bootstrap 核心 CSS 文件 -->
<link rel="stylesheet" href="http://cdn.bootcss.com/bootstrap/3.3.0/css/bootstrap.min.css">
<!-- 可选的Bootstrap主题文件（一般不用引入） -->
<link rel="stylesheet" href="http://cdn.bootcss.com/bootstrap/3.3.0/css/bootstrap-theme.min.css">
<!-- jQuery文件。务必在bootstrap.min.js 之前引入 -->
<link rel="stylesheet" href="css/style.css" media="screen" type="text/css" />

<link type="text/css" rel="stylesheet" href="./css/carousel.css">

<script type="text/javascript" src="./js/jquery.js"></script>

<script type="text/javascript" src="./js/carousel.js"></script>

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
    top:370px 
  }
  .image_show{
margin-left: auto;
margin-right: auto;
    //margin-left:100px; 
 }
  .body{
    background-image: url("");
  }
  .text{
    text-align: center;       
  }
</style>
</head>

<body class = "body">
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









<div id="container">
  <p><a href="#">
    OCR演示
  </a></p>
  <div style="text-align:center;clear:both">
    <script src="/gg_bd_ad_720x90.js" type="text/javascript"></script>
    <script src="/follow.js" type="text/javascript"></script>
  </div>
</div>
<script src="js/index.js"></script>



<p><font size="5px">点击图片进行识别</font></p>





<div class="J_Poster poster-main image_show" data-setting='{

"width":800,
"height":500,
"posterWidth":700,
"posterHeight":500,
"scale":0.8,
"autoPlay":true,
"delay":2000,
"speed":300
}'>
    <div class="poster-btn poster-prev-btn"></div>
       <ul class="poster-list">
         <li class="poster-item"><a href="<?php $file_name = "./img/eng_1.png" ; $lan_type = "eng"; echo "./intro_deal.php?file_name=".$file_name."&lan_type=". $lan_type;?>"><img src="<?php echo $file_name?>" width="100%" height="100%"></a></li>
         <li class="poster-item"><a href="<?php $file_name = "./img/eng_2.png" ; $lan_type = "eng"; echo "./intro_deal.php?file_name=".$file_name."&lan_type=". $lan_type;?>"><img src="<?php echo $file_name?>" width="100%" height="100%"></a></li>
         <li class="poster-item"><a href="<?php $file_name = "./img/eng_3.png" ; $lan_type = "eng"; echo "./intro_deal.php?file_name=".$file_name."&lan_type=". $lan_type;?>"><img src="<?php echo $file_name?>" width="100%" height="100%"></a></li>
        <li class="poster-item"><a href="<?php $file_name = "./img/chi_sim1.png" ; $lan_type = "chi_sim"; echo "./intro_deal.php?file_name=".$file_name."&lan_type=". $lan_type;?>"><img src="<?php echo $file_name?>" width="100%" height="100%"></a></li>
        <li class="poster-item"><a href="<?php $file_name = "./img/chi_sim_small1.jpg" ; $lan_type = "chi_sim"; echo "./intro_deal.php?file_name=".$file_name."&lan_type=". $lan_type;?>"><img src="<?php echo $file_name?>" width="100%" height="100%"></a></li>
        <li class="poster-item"><a href="<?php $file_name = "./img/chi_sim3.png" ; $lan_type = "chi_sim"; echo "./intro_deal.php?file_name=".$file_name."&lan_type=". $lan_type;?>"><img src="<?php echo $file_name?>" width="100%" height="100%"></a></li>
       </ul>
    <div class="poster-btn poster-next-btn"></div>
</div>

<!--<p style="height:50px;"></p>

<div class="J_Poster poster-main" data-setting='{
"width":800,
"height":500,
"posterWidth":640,
"posterHeight":270,
"scale":0.8,
"autoPlay":true,
"delay":5000,
"speed":300,
"vericalAlign":"top"
}'>
<div class="poster-btn poster-prev-btn"></div>
    <ul class="poster-list">
        <li class="poster-item"><a href="#"><img src="./img/1.jpg" width="100%" height="100%"></a></li>
        <li class="poster-item"><a href="#"><img src="./img/2.jpg" width="100%" height="100%"></a></li>
        <li class="poster-item"><a href="#"><img src="./img/3.jpg" width="100%" height="100%"></a></li>
        <li class="poster-item"><a href="#"><img src="./img/4.jpg" width="100%" height="100%"></a></li>

    </ul>
<div class="poster-btn poster-next-btn"></div>
</div>

-->
<script>
$(function(){
Carousel.init($(".J_Poster"));
});
</script>



</body>
</html>
