<center>
<div id="user-login-form">
		<div class="panel-col-first">
			<div class="inside">
			     <?php   print "<p align=\"Center\"> <font color=\"black\" size=\"4\"><b>LOGIN</b></font></p>"; ?>
				  <?php 
				  $form['name']['#title'] = 'Username';
				  $form['name']['#description'] = '';
				  $form['name']['#size'] = '30';
				  $form['pass']['#size'] = '30';
				  $form['pass']['#description'] = '';
				  print drupal_render($form['name']); ?>
				  <?php print drupal_render($form['pass']); ?>
				  <?php
					print drupal_render($form['form_build_id']);
					print drupal_render($form['form_id']); 
					print drupal_render($form['actions']);
					print drupal_render($form);
				  ?>
			</div>
		</div>
		<div class="panel-col">
			<div class="inside">
				<?php
				  print "<p align=\"justify\"> <font color=\"white\" face=\"times\" size=\"3\">Knownet facilitates easy consolidation of communication and exchange of knowledge among collaborators and in a social manner.

					<br><br>Knownet's mission is to collect and organise collective knowledge and make it accessible and useful to collaborators.</font></p>"; // print the password field
				?>
			 </div> 
		</div>
</div>
</center>
