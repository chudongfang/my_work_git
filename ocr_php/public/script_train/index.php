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
<link rel="stylesheet" type="text/css" href="css/normalize.css" />
<link rel="stylesheet" type="text/css" href="css/default.css" />
<link rel="stylesheet" type="text/css" href="css/component.css" />

<link href="css/styles.css" rel="stylesheet" />
<link href="css/inserthtml.com.radios.css" rel="stylesheet" type="text/css"/>
<link rel="stylesheet" type="text/css" href="css/default.css">
<script src="js/modernizr.js"></script>




<!-- jQuery文件。务必在bootstrap.min.js 之前引入 -->
<script src="http://cdn.bootcss.com/jquery/1.11.1/jquery.min.js"></script>

<!-- 最新的 Bootstrap 核心 JavaScript 文件 -->
<script src="http://cdn.bootcss.com/bootstrap/3.3.0/js/bootstrap.min.js"></script>

<style type="text/css">
  .form{
    float:right;
    width:45%;
    margin-top:2%;
  }
  .image{
    float:left;
    width:45%;
    height:300px;
    padding-top:2%;
    padding-left:4%;  
}
  .retext{
     float:left;
     width:92%;
     margin-right:4%;
     margin-left:4%;
  }
  .body{
    background-image: url("./img/background.jpg");
  }
  .text{
    text-align: center;       
  }
</style>
<!--<script src="js/base-loading.js"></script>-->
<script type="text/javascript">
function checkdata1()
{
  if(document.form.file.value == "")
  {
     alert("请选择图片!");
     return false;
  }
  if(var s = document.form.mrbook.value =="")
  {
     alert(s);
     return false;
  }
  
  alert("请确保您上传的为图片类型！");
}
</script>

<script>(function(e,t,n){var r=e.querySelectorAll("html")[0];r.className=r.className.replace(/(^|\s)no-js(\s|$)/,"$1js$2")})(document,window,0);</script>

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


<div class="container">

    <div class="row">


<?php
?>

<?php

if(!empty($_FILES['file']))
{

    $file_path = sprintf("./upload/%s",$_FILES['file']['name']);
    $_file_path = sprintf("./upload/_%s",$_FILES['file']['name']);
    if(!move_uploaded_file($_FILES["file"]["tmp_name"],
        $file_path))
        echo $_FILES["file"]["error"];
    $i=0;
    echo count($_POST[mrbook]);
    if(strcmp($_POST[mrbook][0],"图像优化") == 0)
    {
        $i = 1;
        $cmd = "./imgpro/textcleaner -g -e stretch -f 25 -o 10 -u -s 1 -T -p 10 " . $file_path . " " . $_file_path;
        $file_path = $_file_path;
        shell_exec($cmd);
    }
    for(;$i<count($_POST[mrbook]);$i++)
    {   
        if(strcmp($_POST[mrbook][$i],"English") == 0)
        {
            $tessdata = array();
            $dir = "./tessdata/";
            //  Open a known directory, and proceed to read its contents
            if (is_dir($dir)) {
                if ($dh = opendir($dir)) {
                    $i=0;
                    while (($file = readdir($dh)) !== false) {
                        if($file[0]=='e' && $file[1]=='n' && $file[2]=='g')
                            $tessdata[$i++] = $file;
                    } 
                    closedir($dh);
                }
            }
            for($i = 0 ; $i < count($tessdata);$i++)
            {

                $len = strlen($temp);
                $temp = strtok($tessdata[$i], ".");
                $tessdata[$i] = sprintf("%s",$temp);
                if(count($lan_type)==0)
                    $lan_type = sprintf(" -l %s",$temp);
                else
                    $lan_type = sprintf("%s+%s",$lan_type,$temp);
            }
        }
        else if(strcmp($_POST[mrbook][$i],"中文") == 0)
        {
            $tessdata = array();
            $dir = "./tessdata/";
            //  Open a known directory, and proceed to read its contents
            if (is_dir($dir)) {
                if ($dh = opendir($dir)) {
                    $i=0;
                    while (($file = readdir($dh)) !== false) {
                        if($file[0]=='c' && $file[1]=='h' && $file[2]=='i')
                            $tessdata[$i++] = $file;
                    } 
                    closedir($dh);
                }
            }
            for($i = 0 ; $i < count($tessdata);$i++)
            {

                $len = strlen($temp);
                $temp = strtok($tessdata[$i], ".");
                $tessdata[$i] = sprintf("%s",$temp);
                if(count($lan_type)==0)
                    $lan_type = sprintf(" -l %s",$temp);
                else
                    $lan_type = sprintf("%s+%s",$lan_type,$temp);
            }
        }
    }
    require '../vendor/autoload.php';
    $ocr = new \TesseractOCR($file_path);
    $string = $ocr ->lang($lan_type) ->run();
}


?>
     <div class="col-md-5">
       <div>
         <p>
         <img src=<?php
          if(!empty($file_path)) echo $file_path; 
          else  echo './img/180.jpg'; ?>
            width=100% height=100% />
         </p>
       </div>
     </div>


    <div class="col-md-5 ">
       <div >
  <form  action="" method="post" enctype="multipart/form-data" name = "form" OnSubmit="checkdata1();">
        <br/>
           <input type="file" name="file" id="file-1" class="inputfile inputfile-1"/>
    <label for="file-1"><svg xmlns="http://www.w3.org/2000/svg" width="150" height="100" viewBox="0 0 20 17"><path d="M10 0l-5.2 4.9h3.3v5.1h3.8v-5.1h3.3l-5.2-4.9zm9.3 11.5l-3.2-2.1h-2l3.4 2.6h-3.5c-.1 0-.2.1-.2.1l-.8 2.3h-6l-.8-2.2c-.1-.1-.1-.2-.2-.2h-3.6l3.4-2.6h-2l-3.2 2.1c-.4.3-.7 1-.6 1.5l.6 3.1c.1.5.7.9 1.2.9h16.3c.6 0 1.1-.4 1.3-.9l.6-3.1c.1-.5-.2-1.2-.7-1.5z"/></svg> <span>选择文件&hellip;</span></label>
<!--<label for="file">上传图片:</label>
           <input type="file" name="file" id="file" /> 
           <br />
           <p>请选择语言类型(可多选)：</p> 
         <div class="mZ">
           <label>
             <input type="checkbox" value="English" name ="mrbook[]" >
             English
          </label>
         </div>
         <div class="checkbox">
           <label>
           <input type="checkbox" value="中文" name ="mrbook[]">
           中文
           </label>
          </div>

          <div class="checkbox">
            <label>
              <input type="checkbox" value="Deutsch" name ="mrbook[]">
              Deutsch
           </label>
          </div>
          <div class="checkbox">
            <label>
             <input type="checkbox" value="한국의" name ="mrbook[]">
             한국의
            </label>
          </div>-->
              <div  style="width: 200px;">
              <input type="checkbox" id="checkbox-7-1.1" name = "mrbook[]" value = "图像优化"/>
               <label for="checkbox-7-1.1">
                   <span>图像优化</span>
               </label>
              <input type="checkbox" id="checkbox-7-1.2" name = "mrbook[]" value = "中文"/>
               <label for="checkbox-7-1.2">
                   <span>中文</span>
               </label>
              <input type="checkbox" id="checkbox-7-1.3" name = "mrbook[]" value = "English"/>
               <label for="checkbox-7-1.3">
                   <span>English</span>
               </label>
              <input type="checkbox" id="checkbox-7-1.4" name = "mrbook[]" value = "Deutsch"/>
               <label for="checkbox-7-1.4">
                   <span>
                   Deutsch
                  </span>
               </label>
              <input type="checkbox" id="checkbox-7-1.5" name = "mrbook[]"/>
               <label for="checkbox-7-1.5">
                   <span>
                   한국의
                  </span>
               </label>
             </div>
         <input type="submit" name="submit" class="btn btn-success btn-lg" value="上传" />
       </form>
    </div>
   </div>
   </div>
</div>

<!--<div class = "btn"><a href="./train.php" class="btn btn-primary btn-lg active" role="button">点我训练^_^</a></div>-->
<div class = "retext" >
<p>识别结果：</p>
    <textarea id="wn" class="form-control" rows="10" ><?php echo $string; ?></textarea>
</div>


</body>
</html>
